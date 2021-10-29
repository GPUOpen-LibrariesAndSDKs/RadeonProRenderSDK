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

//
// This demo covers vertices deformation over time for a blur effect.
//

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any rpr API calls
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
		{ -1.0f, 1.0f, -1.0f,    0.f, 1.f, 0.f,    0.f, 0.f },
		{  1.0f, 1.0f, -1.0f,    0.f, 1.f, 0.f,    1.f, 0.f },
		{  1.0f, 1.0f, 1.0f ,    0.f, 1.f, 0.f,    1.f, 1.f },
		{  -1.0f, 1.0f, 1.0f ,   0.f, 1.f, 0.f,    0.f, 1.f},

		{  -1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f,   0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  0.f, -1.f, 0.f,   1.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,   0.f, -1.f, 0.f,   1.f, 1.f },
		{  -1.0f, -1.0f, 1.0f ,  0.f, -1.f, 0.f,   0.f, 1.f },

		{  -1.0f, -1.0f, 1.0f ,  -1.f, 0.f, 0.f,   0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f , -1.f, 0.f, 0.f,   1.f, 0.f },
		{  -1.0f, 1.0f, -1.0f ,  -1.f, 0.f, 0.f,   1.f, 1.f },
		{  -1.0f, 1.0f, 1.0f ,   -1.f, 0.f, 0.f,   0.f, 1.f },

		{  1.0f, -1.0f, 1.0f ,   1.f, 0.f, 0.f,    0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  1.f, 0.f, 0.f,    1.f, 0.f },
		{  1.0f, 1.0f, -1.0f ,   1.f, 0.f, 0.f,    1.f, 1.f },
		{  1.0f, 1.0f, 1.0f ,    1.f, 0.f, 0.f,    0.f, 1.f },

		{  -1.0f, -1.0f, -1.0f , 0.f, 0.f, -1.f ,  0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f ,  1.f, 0.f },
		{  1.0f, 1.0f, -1.0f ,   0.f, 0.f, -1.f,   1.f, 1.f },
		{  -1.0f, 1.0f, -1.0f ,  0.f, 0.f, -1.f,   0.f, 1.f },

		{  -1.0f, -1.0f, 1.0f ,  0.f, 0.f, 1.f,    0.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,   0.f, 0.f,  1.f,   1.f, 0.f },
		{  1.0f, 1.0f, 1.0f ,    0.f, 0.f, 1.f,    1.f, 1.f },
		{  -1.0f, 1.0f, 1.0f ,   0.f, 0.f, 1.f,    0.f, 1.f },


		// vertices at camera exposure = 1.0 : slightly deform the Cube
		{ -1.0f+shiftX, 1.0f, -1.0f,           0.f, 1.f, 0.f,      0.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f,    0.f, 1.f, 0.f,      1.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f ,    0.f, 1.f, 0.f,      1.f, 1.f },
		{  -1.0f+shiftX, 1.0f, 1.0f ,          0.f, 1.f, 0.f,      0.f, 1.f},

		{  -1.0f, -1.0f, -1.0f ,               0.f, -1.f, 0.f,     0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,                0.f, -1.f, 0.f,     1.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,                 0.f, -1.f, 0.f,     1.f, 1.f },
		{  -1.0f, -1.0f, 1.0f ,                0.f, -1.f, 0.f,     0.f, 1.f },

		{  -1.0f, -1.0f, 1.0f ,               -1.f, 0.f, 0.f,      0.f, 0.f },
		{  -1.0f, -1.0f, -1.0f ,              -1.f, 0.f, 0.f,      1.f, 0.f },
		{  -1.0f+shiftX, 1.0f, -1.0f ,        -1.f, 0.f, 0.f,      1.f, 1.f },
		{  -1.0f+shiftX, 1.0f, 1.0f ,         -1.f, 0.f, 0.f,      0.f, 1.f },

		{  1.0f, -1.0f, 1.0f ,                1.f, 0.f, 0.f,       0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,               1.f, 0.f, 0.f,       1.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f ,  1.f, 0.f, 0.f,       1.f, 1.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f ,   1.f, 0.f, 0.f,       0.f, 1.f },

		{  -1.0f, -1.0f, -1.0f ,              0.f, 0.f, -1.f ,     0.f, 0.f },
		{  1.0f, -1.0f, -1.0f ,               0.f, 0.f, -1.f ,     1.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, -1.0f ,  0.f, 0.f, -1.f,      1.f, 1.f },
		{  -1.0f+shiftX, 1.0f, -1.0f ,        0.f, 0.f, -1.f,      0.f, 1.f },

		{  -1.0f, -1.0f, 1.0f ,               0.f, 0.f, 1.f,       0.f, 0.f },
		{  1.0f, -1.0f, 1.0f ,                0.f, 0.f,  1.f,      1.f, 0.f },
		{  1.0f+shiftX, 1.0f+shiftY, 1.0f ,   0.f, 0.f, 1.f,       1.f, 1.f },
		{  -1.0f+shiftX, 1.0f, 1.0f ,         0.f, 0.f, 1.f,       0.f, 1.f },

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

		// Set light radiant power
		CHECK(rprPointLightSetRadiantPower3f(light, 200, 200, 200));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}

	// apply material on cube
	rpr_material_node diffuseA = nullptr;
	rpr_material_node diffuseB = nullptr;
	rpr_material_node uv_node = nullptr;
	rpr_material_node uv_scaled_node = nullptr;
	rpr_material_node checker = nullptr;
	rpr_material_node layered = nullptr;
	{
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseA));
		CHECK( rprMaterialNodeSetInputFByKey(diffuseA, RPR_MATERIAL_INPUT_COLOR, 0.0f, 0.5f, 1.0f, 0.0f)); // Diffuse color

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseB));
		CHECK( rprMaterialNodeSetInputFByKey(diffuseB, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.20f, 1.0f, 0.0f)); // Diffuse color

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node));
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV));

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node));
		CHECK( rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL));
		CHECK( rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node));
		CHECK( rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 0.7f, 0.7f, 0.0f, 0));

		CHECK(  rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_CHECKER_TEXTURE, &checker)); 
		CHECK(  rprMaterialNodeSetInputNByKey(checker, RPR_MATERIAL_INPUT_UV  , uv_scaled_node));

		// Create layered material
		CHECK(  rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_BLEND, &layered));
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR0, diffuseA)); // Set material for base layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR1, diffuseB)); // Set material for top layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_WEIGHT, checker));

		CHECK( rprShapeSetMaterial(cube,layered));
	}

	// apply material on floor
	rpr_material_node diffuseC = nullptr;
	{
		CHECK(  rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseC));
		CHECK(  rprMaterialNodeSetInputFByKey(diffuseC, RPR_MATERIAL_INPUT_COLOR, 1.0f, 0.2f, 0.0f, 0.0f)); // Diffuse color

		CHECK( rprShapeSetMaterial(plane,diffuseC));
	}

	// set exposure, for motion blur
	CHECK(rprCameraSetExposure(camera,1.0));

	// Progressively render an image
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) ); // set display gamma
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK( rprFrameBufferClear(frame_buffer) );
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "13.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(diffuseA));diffuseA=nullptr;
	CHECK(rprObjectDelete(diffuseB));diffuseB=nullptr;
	CHECK(rprObjectDelete(uv_node));uv_node=nullptr;
	CHECK(rprObjectDelete(uv_scaled_node));uv_scaled_node=nullptr;
	CHECK(rprObjectDelete(checker));checker=nullptr;
	CHECK(rprObjectDelete(layered));layered=nullptr;
	CHECK(rprObjectDelete(diffuseC));diffuseC=nullptr;
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


