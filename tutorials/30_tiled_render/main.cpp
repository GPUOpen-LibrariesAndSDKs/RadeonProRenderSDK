/*****************************************************************************\
*
*  Module Name    Tiled Rendering
*  Project        Radeon pro render rendering tutorial
*
*  Description    How to render a scene using tiles. This doesn't require you to
				  allocate the full framebuffer.
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "RprLoadStore.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../3rdParty/stbi/stbi.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <string>


//
// This demo illustrates how to break down the framebuffer into smaller render regions (tiles)
//
//



//-------------------Utilities-----------------------------
static void StudyErrorCode(rpr_int errorCode, rpr_context context__ = nullptr)
{
	if (errorCode != RPR_SUCCESS && context__)
	{
		rpr_int status = 0;
		size_t sizeParamA = 0;
		status = rprContextGetInfo(context__, RPR_CONTEXT_LAST_ERROR_MESSAGE, 0, 0, &sizeParamA);
		if (status == RPR_SUCCESS && sizeParamA >= 1)
		{
			char* paramData = new char[sizeParamA];
			status = rprContextGetInfo(context__, RPR_CONTEXT_LAST_ERROR_MESSAGE, sizeParamA, paramData, 0);
			if (status == RPR_SUCCESS)
			{
				printf("ErrorMessage = %s\n", paramData);
			}
			delete[] paramData; paramData = 0;
		}
	}
}

template <typename T> T clamp(T x, T a, T b)
{
	return x < a ? a : (x > b ? b : x);
}

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
//------------------------------------------------------------

//4k resolution
const int RenderTargetSizeX = 3840;
const int RenderTargetSizeY = 3840;

/*
Define a sensor size to match the render aspect ratio.
The width is based off the standard 35mm sensor size, but adjusted to match aspect ratio.
*/
constexpr float SensorY = 36.f;
constexpr float SensorX = SensorY * ((float)RenderTargetSizeX / (float)RenderTargetSizeY);
const int maxIterationRendering = 3;

/*
For very large render targets, it is beneficial to break down the framebuffer into smaller render regions (tiles)
*/
struct sFrameBufferMetadata
{
	int mRenderTargetSizeX, mRenderTargetSizeY;
	int mTileSizeX, mTileSizeY;
	std::vector<rpr_uchar> mData;
};
void rprextMultiTileRender(sFrameBufferMetadata& meta, rpr_scene scene, rpr_context context, rpr_uint maxIterationRendering)
{
	//for obvious reasons...
	CHECK_GT(meta.mRenderTargetSizeX , meta.mTileSizeX);
	CHECK_GT(meta.mRenderTargetSizeY , meta.mTileSizeY);

	meta.mData.resize(meta.mRenderTargetSizeX * meta.mRenderTargetSizeY * 3);

	rpr_int status = RPR_SUCCESS;

	float tilesXf = meta.mRenderTargetSizeX / float(meta.mTileSizeX);
	float tilesYf = meta.mRenderTargetSizeY / float(meta.mTileSizeY);

	int tilesX = (int)ceil(tilesXf);
	int tilesY = (int)ceil(tilesYf);

	printf("info:\n");
	printf("  Virtual resolution: %dx%d\n", meta.mRenderTargetSizeX, meta.mRenderTargetSizeY);
	printf("  Tile resolution:    %dx%d\n", meta.mTileSizeX, meta.mTileSizeY);
	printf("  Tiled resolution:   %fx%f\n", tilesXf, tilesYf);
	printf("  Tiled offset:       %f,%f\n", -tilesXf / 2.0f + .5f, -tilesYf / 2.0f + .5f);

	//allocate the frambuffer data
	rpr_framebuffer_desc desc = { (rpr_uint)meta.mTileSizeX , (rpr_uint)meta.mTileSizeY };
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };

	rpr_framebuffer frame_buffer = NULL; status = rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer); CHECK(status);
	rpr_framebuffer frame_bufferSolved = NULL; status = rprContextCreateFrameBuffer(context, fmt, &desc, &frame_bufferSolved); CHECK(status);
	status = rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer); CHECK(status);

	//Tempory allocation for framebuffer data needed for the stitching phase.
	size_t frame_buffer_dataSize = 0;
	status = rprFrameBufferGetInfo(frame_bufferSolved, RPR_FRAMEBUFFER_DATA, 0, NULL, &frame_buffer_dataSize); CHECK(status);
	CHECK_EQ( frame_buffer_dataSize , meta.mTileSizeX * meta.mTileSizeY * 4 * sizeof(float) );
	float* frame_buffer_data = (float*)malloc(frame_buffer_dataSize);

	//we need to change the camera sensor size. It controls aspect ratios which in turn 
	//defines how rays are being cast. We are effectively tracing subsets of the sensor
	rpr_camera camera = nullptr;
	status = rprSceneGetCamera(scene, &camera); CHECK(status);
	status = rprCameraSetSensorSize(camera, SensorX / tilesXf, SensorY / tilesYf);   CHECK(status);

	//render each tile and blit onto the rtbacking
	float shiftY = -(tilesYf / 2.0f) + 0.5f;//shift the render plane by H/2
	float deltaX = 1;
	float deltaY = 1;
	for (int yTile = 0; yTile < tilesY; yTile++)
	{
		float shiftX = -(tilesXf / 2.0f) + 0.5f;//shift the render plane by W/2
		for (int xTile = 0; xTile < tilesX; xTile++)
		{
			//shift the camera viewport to account for the tile offset
			status = rprCameraSetLensShift(camera, shiftX, shiftY);  CHECK(status);

			status = rprFrameBufferClear(frame_buffer); CHECK(status);
			//render all iterations on the current tile
			for (rpr_uint i = 0; i < maxIterationRendering; ++i)
			{
				// force the framecount, so we ensure each tiles is using the same seed.
				status = rprContextSetParameterByKey1u(context, RPR_CONTEXT_FRAMECOUNT, i); CHECK(status);
				status = rprContextRender(context); CHECK(status);
			}
			status = rprContextResolveFrameBuffer(context, frame_buffer, frame_bufferSolved, false); CHECK(status);

			//read back
			status = rprFrameBufferGetInfo(frame_bufferSolved, RPR_FRAMEBUFFER_DATA, frame_buffer_dataSize, frame_buffer_data, NULL); CHECK(status);

			//stitch current rendered tile data to the FB
			int offsetInRenderTargetX = xTile*meta.mTileSizeX;
			int offsetInRenderTargetY = yTile*meta.mTileSizeY;

			for (unsigned j = 0; j < meta.mTileSizeY; ++j)
			{
				for (unsigned i = 0; i < meta.mTileSizeX; ++i)
				{
					int dstX = i + offsetInRenderTargetX;
					int dstY = j + offsetInRenderTargetY;

					if (dstX >= meta.mRenderTargetSizeX || dstY >= meta.mRenderTargetSizeY)
						continue;

					//Vertical flip source and dest
					int idx = dstX + (meta.mRenderTargetSizeY - dstY - 1) * meta.mRenderTargetSizeX;
					int sIdx = i + (meta.mTileSizeY - j - 1) * meta.mTileSizeX;

					//normalization (divide by number of samples)
					float invW = 1.f / frame_buffer_data[4 * sIdx + 3];

					meta.mData[3 * idx + 0] = clamp(int(frame_buffer_data[4 * sIdx] * 255.f), 0, 255);
					meta.mData[3 * idx + 1] = clamp(int(frame_buffer_data[4 * sIdx + 1] * 255.f), 0, 255);
					meta.mData[3 * idx + 2] = clamp(int(frame_buffer_data[4 * sIdx + 2] * 255.f), 0, 255);
				}
			}

			shiftX += deltaX;
		}
		shiftY += deltaY;
	}

	free(frame_buffer_data); frame_buffer_data = nullptr;
	if (frame_buffer) { status = rprObjectDelete(frame_buffer); frame_buffer = NULL; CHECK(status); }
	if (frame_bufferSolved) { status = rprObjectDelete(frame_bufferSolved); frame_bufferSolved = NULL; CHECK(status); }
}

//#define NO_TILE //<--- Uncomment if you want to render to a full FB
int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any rpr API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	// the RPR context object.
	rpr_context context = nullptr;

	// Register the RPR DLL
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1);
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	// note that multiple GPUs can be enabled for example with creation_flags = RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_GPU1
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set the active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "RPR Context creation succeeded." << std::endl;


	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	rpr_scene scene = NULL;
	CHECK( rprsImport("../../Resources/Meshes/matball.rprs", context, matsys, &scene, false, nullptr));


#ifdef NO_TILE
	{
		rpr_camera camera = nullptr;
		status = rprSceneGetCamera(scene, &camera); CHECK(status);
		status = rprCameraSetSensorSize(camera, SensorX, SensorY);   CHECK(status);

		rpr_framebuffer_desc desc;
		desc.fb_width = RenderTargetSizeX;
		desc.fb_height = RenderTargetSizeY;
		rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };

		rpr_framebuffer frame_buffer = NULL; status = rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer); CHECK(status);
		rpr_framebuffer frame_bufferSolved = NULL; status = rprContextCreateFrameBuffer(context, fmt, &desc, &frame_bufferSolved); CHECK(status);
		status = rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer); CHECK(status);

		status = rprFrameBufferClear(frame_buffer); CHECK(status);
		for (rpr_uint i = 0; i < maxIterationRendering; ++i)
		{
			status = rprContextSetParameterByKey1u(context, RPR_CONTEXT_FRAMECOUNT, i); CHECK(status); // force the framecount, so we ensure each tiles is using the same seed.
			status = rprContextRender(context); CHECK(status);
		}
		rprContextResolveFrameBuffer(context, frame_buffer, frame_bufferSolved, false);

		status = rprFrameBufferSaveToFile(frame_bufferSolved, "feature_multiTile_no_tile.png"); CHECK(status);

		if (frame_buffer) { status = rprObjectDelete(frame_buffer); frame_buffer = NULL; CHECK(status); }
		if (frame_bufferSolved) { status = rprObjectDelete(frame_bufferSolved); frame_bufferSolved = NULL; CHECK(status); }
	}
#else
	sFrameBufferMetadata meta;
	meta.mRenderTargetSizeX = RenderTargetSizeX;
	meta.mRenderTargetSizeY = RenderTargetSizeY;
	for (int i = 128; i <= 512; i *= 2)
	{
		meta.mTileSizeX = i;
		meta.mTileSizeY = i;
		rprextMultiTileRender(meta, scene, context, maxIterationRendering);
		if (!stbi_write_png((std::string("30_tiled_render") + std::to_string(i) + ".png").c_str(), RenderTargetSizeX, RenderTargetSizeY, 3, &meta.mData[0], RenderTargetSizeX * 3)) 
		{ 
			CHECK_EQ(0,1);
		}
	}
#endif

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	if (scene) { CHECK( rprObjectDelete(scene)); }
	if (matsys) { CHECK( rprObjectDelete(matsys)); }
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}