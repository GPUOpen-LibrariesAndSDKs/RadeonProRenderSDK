/*****************************************************************************\
*
*  Module Name    Framebuffer access demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>

//
// Here we demonstrate usage of rprFrameBufferGetInfo: We can access the image data of a frame buffer
// In this demo, we use the data of a first rendering and use it as an input texture for a second rendering.
// It also contains an example of DeepEXR framebuffer export.
//


RPRGarbageCollector g_gc;


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

	rpr_material_system matsys=nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// Create a scene
	rpr_scene scene=nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create point light
	rpr_light light=nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));
		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 10));
		// Set transform for the light
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));
		// Set light radiant power
		CHECK(rprPointLightSetRadiantPower3f(light, 80, 80, 80));
		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}

	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 0.8f) );

	// Create camera
	rpr_camera camera=nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		CHECK( rprCameraLookAt(camera, 5, 5, 20,  0, 0, 0,   0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 10.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );

		
	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 1.0f, 1.0f)  );


	// Create cube mesh
	rpr_shape cube=nullptr;
	{
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube));

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 3, 0)) * RadeonProRender::scale(RadeonProRender::float3(3.0f, 3.0f, 3.0f));
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));
	}


	// Create simple diffuse shader
	rpr_material_node diffuse=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse));

		// Set diffuse color parameter
		CHECK(rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 1.0f, 0.5f, 1.f));

		// Set shader for cube & plane meshes
		CHECK(rprShapeSetMaterial(cube, diffuse));

	}

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	g_gc.GCAdd(frame_buffer);
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	g_gc.GCAdd(frame_buffer_resolved);

	
	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,200));
	CHECK(rprFrameBufferClear(frame_buffer));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	// This can be uncommented to see the framebuffer
	// CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "31_temp.png") );


	//
	// Demo for DeepEXR export.
	//
	// This part of code can be ignored ( or commented-out ) if the reader is not interested by DeepEXR: it doesn't affect the rest.
	{
		rpr_framebuffer_format fmt_deepEXR = { 0, RPR_COMPONENT_TYPE_DEEP }; // note that rpr_framebuffer_format::num_components is not needed for deepEXR.
		rpr_framebuffer frame_buffer_deepEXR = NULL; 
		CHECK( rprContextCreateFrameBuffer(context, fmt_deepEXR, &desc, &frame_buffer_deepEXR));
		g_gc.GCAdd(frame_buffer_deepEXR);
		CHECK( rprContextSetAOV(context, RPR_AOV_DEEP_COLOR, frame_buffer_deepEXR));

		// MERGE_Z_THRESHOLD : Merge close samples in order to reduce the EXR file size. Any positive value can be set.
		CHECK( rprContextSetParameterByKey1f(context,  RPR_CONTEXT_DEEP_SUBPIXEL_MERGE_Z_THRESHOLD,  0.1f)); 

		// GPU_ALLOCATION_LEVEL : this sets the speed/memory usage trade-off on GPU when generating deepEXR. Should be 1~32.
		// It's advised to use 4 ( current default value )
		// Modifying this value do NOT change the generated deepEXR file, it's only about performance.
		CHECK( rprContextSetParameterByKey1u(context,  RPR_CONTEXT_DEEP_GPU_ALLOCATION_LEVEL,  4));

		// COLOR_ENABLED can be set to 0 in order to remove RGB data from the deepEXR. Advantages are smaller EXR file and faster generation.
		CHECK( rprContextSetParameterByKey1u(context,  RPR_CONTEXT_DEEP_COLOR_ENABLED,  1));

		// Render the scene
		CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,100)); // increasing interation makes bigger deepEXR file.
		CHECK(rprFrameBufferClear(frame_buffer));
		CHECK( rprContextRender(context) );

		CHECK( rprFrameBufferSaveToFile(frame_buffer_deepEXR, "31_deepEXR.exr"));

		std::cout << "DeepEXR exported." << std::endl;
	}



	///////// Framebuffer Access Tutorial //////////
	//
	// We are going to take the  frame_buffer_resolved data,  and use it as a material texture for the cube.

	// access framebuffer data.
	size_t size = 0;
	CHECK(rprFrameBufferGetInfo(frame_buffer_resolved, RPR_FRAMEBUFFER_DATA, 0, NULL, &size));
	float* buffer = new float[size / sizeof(float)];
	CHECK(rprFrameBufferGetInfo(frame_buffer_resolved, RPR_FRAMEBUFFER_DATA, size, buffer, 0));


	//Apply this data as a texture material to the cube.

	rpr_material_node diffuse1=nullptr;
	rpr_material_node tex=nullptr;
	rpr_image img1=nullptr;
	{
		// create the image from the data of the previous framebuffer
		rpr_image_format format;
		format.num_components = 4;
		format.type = RPR_COMPONENT_TYPE_FLOAT32;
		rpr_image_desc desc2;
		desc2.image_width = desc.fb_width;
		desc2.image_height = desc.fb_height;
		desc2.image_row_pitch = 0;
		desc2.image_slice_pitch = 0;
		desc2.image_depth = 0;
		CHECK(rprContextCreateImage(context, format, &desc2, buffer, &img1));

		// gamma of the read image is 2.2
		CHECK( rprImageSetGamma(img1, 2.2));

		// create the image sampler material
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &tex));

		// Set image data
		CHECK(rprMaterialNodeSetInputImageDataByKey(tex, RPR_MATERIAL_INPUT_DATA, img1));
		
		// create a simple diffuse material
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));

		// apply the built texture as color
		CHECK(rprMaterialNodeSetInputNByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, tex));

		// cube now uses this diffuse as material.
		CHECK(rprShapeSetMaterial(cube, diffuse1));
	}
	
	// change camera : getting closer to the cube
	CHECK( rprCameraSetFocalLength(camera, 75.f) );
	CHECK( rprCameraLookAt(camera, 30, 14, 30,  0, 2.5, 0,   0, 1, 0) );


	// Render the scene
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,200));
	CHECK(rprFrameBufferClear(frame_buffer));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	std::cout << "Rendering finished.\n";

	//delete buffer;
	delete[] buffer; buffer=nullptr;

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "31.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(tex));tex=nullptr;
	CHECK(rprObjectDelete(img1));img1=nullptr;
	CHECK(rprObjectDelete(diffuse1));diffuse1=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(cube));cube=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}
