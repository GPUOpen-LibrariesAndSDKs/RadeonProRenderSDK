/*****************************************************************************\
*
*  Module Name    Deformation Motion Demo
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

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any rpr API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK simple rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create OpenCL context using a single GPU 
	rpr_context context = NULL;

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "Context successfully created.\n";

	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create camera
	rpr_camera camera = nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		CHECK( rprCameraLookAt(camera, 0, 5, 20,   0, 1, 0,    0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800 , 600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer to black color
	CHECK( rprFrameBufferClear(frame_buffer) );

	// Set framebuffer for the context
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );

	const float shiftX = 0.3f;
	const float shiftY = 0.3f;

	const int cube_NumberOfVertices = 24;
	const int numberOfBlurKeyTime = 2;

	// Cube geometry
	vertex cube_data_motionBlur[cube_NumberOfVertices*numberOfBlurKeyTime] = 
	{
		// vertices at camera exposure = 0.0
		{ -1.0f, 1.0f, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
		{  1.0f, 1.0f, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
		{  1.0f, 1.0f, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f },
		{  -1.0f, 1.0f, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f},
		{  -1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, 1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, 1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, 1.0f, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, 1.0f, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
		{  1.0f, 1.0f, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },
		{  -1.0f, 1.0f, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f , 0.f, 0.f,  1.f, 0.f, 0.f },
		{  1.0f, 1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
		{  -1.0f, 1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },

		// vertices at camera exposure = 1.0 : slightly deform the Cube
		{ -1.0f+shiftX, 1.0f, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f },
		{  -1.0f+shiftX, 1.0f, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f},
		{  -1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f+shiftX, 1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f+shiftX, 1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },
		{  -1.0f+shiftX, 1.0f, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },
		{  -1.0f, -1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
		{  1.0f, -1.0f, 1.0f , 0.f, 0.f,  1.f, 0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
		{  -1.0f+shiftX, 1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },

	};


	// Create cube mesh
	rpr_shape cube = nullptr;
	{
		// 0-terminated list of mesh extra properties.  (key+value)
		rpr_mesh_info mesh_properties[16];

		// first key: specify that we want to use mesh deformation on this rpr_shape.
		mesh_properties[0] = (rpr_mesh_info)RPR_MESH_MOTION_DIMENSION;

		// 2 key times are used in cube_data_motionBlur ( exposure = 0.0 and  exposure = 1.0 )
		// More key times can be used if needed, for example, 3 key times would mean :  exposure = 0.0; 0.5; 1.0
		mesh_properties[1] = (rpr_mesh_info)numberOfBlurKeyTime;

		mesh_properties[2] = (rpr_mesh_info)0; // key=0 means end of mesh properties.

		const rpr_float* texcoords[] = {   (rpr_float const*)((char*)&cube_data_motionBlur[0] + sizeof(rpr_float)*6)	 }; 
		size_t num_texcoords[] = { cube_NumberOfVertices*numberOfBlurKeyTime }; 
		rpr_int texcoord_stride[] = { sizeof(vertex)  }; 
		const rpr_int* texcoord_indices_[] = { indices }; 
		rpr_int tidx_stride_[] = { sizeof(rpr_int) }; 

		CHECK(rprContextCreateMeshEx2(context,
			(rpr_float const*)&cube_data_motionBlur[0], cube_NumberOfVertices*numberOfBlurKeyTime, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data_motionBlur[0] + sizeof(rpr_float) * 3), cube_NumberOfVertices*numberOfBlurKeyTime, sizeof(vertex),
			nullptr,0,0,
			1, // using 1 texture coord layer.
			texcoords, num_texcoords, texcoord_stride,
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			texcoord_indices_,tidx_stride_,
			num_face_vertices, 12, mesh_properties, &cube));

		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1, 0));

		// Set the transform 
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
	}

	// Create plane mesh
	rpr_shape plane = nullptr;
	{
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &plane));

		// Add plane into the scene
		CHECK(rprSceneAttachShape(scene, plane));
	}

	// Create point light
	rpr_light light=nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 2));

		// Set transform for the light
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light radiant power in Watts
		CHECK(rprPointLightSetRadiantPower3f(light, 100, 100, 100));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}

	// set exposure, for motion blur
	CHECK(rprCameraSetExposure(camera,1.0));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "13.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(plane));plane=nullptr;
	CHECK(rprObjectDelete(cube));cube=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


