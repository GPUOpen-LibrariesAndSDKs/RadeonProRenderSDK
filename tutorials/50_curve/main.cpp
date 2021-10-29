/*****************************************************************************\
*
*  Module Name    Curve Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
// Demo covering Curves rendering. 
// Curves are often used for hair,grass.. rendering.
//

#include "RadeonProRender.h"
#include "RprLoadStore.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

RPRGarbageCollector g_gc;


// return a random float between 0.0 and 1.0
float draw()
{
	return (rand()&RAND_MAX)/(double)RAND_MAX;
}

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "-- Radeon ProRender SDK Demo --" << std::endl;

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


	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create point light
	rpr_light light = nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 3));

		// Set transform for the light
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light radiant power
		CHECK(rprPointLightSetRadiantPower3f(light, 255.0f*0.9f, 241.0f*0.9f, 224.0f*0.9f));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}

	// Create camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 0, 5, 20, 0, 1, 0, 0, 1, 0) );
	CHECK( rprCameraSetFocalLength(camera, 75.f) );
	CHECK( rprSceneSetCamera(scene, camera) );
	CHECK( rprContextSetScene(context, scene) );


	CreateAMDFloor(context, scene, matsys, g_gc, 1.0, 1.0);

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );



	// Set framebuffer for the context
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	/////////Curve Tutorial//////////
	
	//It's BCurve model of 4 point
	//It works with StartPoint CtrlPoint CtrlPoint EndPoint
	rpr_curve newCurve = 0;
	
	const float shiftFactor = 0.2f; //  0.0:curves are aligned   -  if increased, curves are spreading on the grid

	int nPoints = 0;
	const int n = 12;
	std::vector<float> cps;
	std::vector<rpr_uint> indices;
	std::vector<float> radius;
	std::vector<rpr_int> segsPerCurve;
	std::vector<float> uvs;
	for(int j=0; j<n; j++) for(int i=0; i<n; i++)
	{
		const float width = 5.f;
		float x = (i/(float)n - 0.5f)*width  +(draw()-0.5f)*shiftFactor;
		float z = (j/(float)n - 0.5f)*width  +(draw()-0.5f)*shiftFactor;

		const float length = 0.1f;
		int nSegs = 8;
		float x0 = x + (draw()-0.5f)*width/n/2;
		float z0 = z + (draw()-0.5f)*width/n/2;
		for(int l=0; l<nSegs; l++)
		{
			for(int k=0; k<4; k++)
			{
				if( k!=0 )
				{
					x0 = x + (draw()-0.5f)*width/n/2;
					z0 = z + (draw()-0.5f)*width/n/2;
				}
				cps.push_back( x0 );
				cps.push_back( (3*l + k) * length );
				cps.push_back( z0 );

				indices.push_back( indices.size() );
				nPoints++;
			}
		}
		radius.push_back( 0.01f );
		segsPerCurve.push_back( nSegs );
		uvs.push_back( i/(float)n );
		uvs.push_back( j/(float)n );
	}
	CHECK(rprContextCreateCurve(
		context,
		&newCurve,
		nPoints, // number of 3d points
		cps.data(), // list of 3d points
		3 * sizeof(float), // stride of 3d points
		indices.size(), // number of indices
		segsPerCurve.size(),	// this  rpr_curve has N curves	
		indices.data(), // indices pointing to controPoint
		radius.data(), // list of radius (float) for each curve
		uvs.data(),     // list of uv (float2) for each curve
		segsPerCurve.data(), // number of segment ( rpr_int ) for each curve
		0
	));

	CHECK(rprObjectSetName(newCurve, "myCurve0"));

	rpr_material_node newDiffuse_red = NULL;
	CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &newDiffuse_red));
	CHECK(rprMaterialNodeSetInputFByKey(newDiffuse_red, RPR_MATERIAL_INPUT_COLOR, 1.0f, 0.5f, 0.5f, 1.f));
	CHECK(rprCurveSetMaterial(newCurve, newDiffuse_red));

	// like any shape, a Curve needs to be attached to scene.
	CHECK(rprSceneAttachCurve(scene, newCurve));

	// set the rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 1.0f ) );

	// Render the scene
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK( rprFrameBufferClear(frame_buffer) );
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "50.png"));

	// Release the stuff we created
	CHECK(rprObjectDelete(newDiffuse_red));newDiffuse_red=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(newCurve));newCurve=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

