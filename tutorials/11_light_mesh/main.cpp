/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>

#define PI 3.1412f

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


	rpr_material_system matsys;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create cube mesh
	rpr_shape cube = nullptr;
	{
		CHECK( rprContextCreateMesh(context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float)*3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float)*6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube) );
		// Add cube into the scene
		CHECK( rprSceneAttachShape(scene, cube) );
		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1, 0));
		// Set the transform 
		CHECK( rprShapeSetTransform(cube, RPR_TRUE, &m.m00) );
	}
	// Create plane mesh
	rpr_shape plane = nullptr;
	{
		CHECK( rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &plane) );

		// Add plane into the scene
		CHECK( rprSceneAttachShape(scene, plane) );
	}

	// Create camera
	rpr_camera camera = nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		// Camera position is (5,5,20)
		// Camera aimed at (0,0,0)
		// Camera up vector is (0,1,0)
		CHECK( rprCameraLookAt(camera, 0, 5, 20, 0, 1, 0, 0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );

	// Create simple diffuse shader
	rpr_material_node diffuse = nullptr;
	{
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse) );

		// Set diffuse color parameter to gray
		CHECK( rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f) );

		// Set shader for cube & plane meshes
		CHECK( rprShapeSetMaterial(cube, diffuse) );

		CHECK( rprShapeSetMaterial(plane, diffuse) );
	}

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

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






	///////// Tutorial Emissive Light //////////

	// Create the light
	rpr_shape light = nullptr;
	rpr_material_node emissive = nullptr;
	{
		CHECK( rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &light) );

		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0,8,2)) * RadeonProRender::rotation( RadeonProRender::float3(0.f,0.f,1.f), PI );

		// Set transform for the light
		CHECK( rprShapeSetTransform(light, RPR_TRUE, &lightm.m00) );

		{
			CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_EMISSIVE, &emissive) );

			// Set diffuse color parameter to gray
			CHECK( rprMaterialNodeSetInputFByKey(emissive, RPR_MATERIAL_INPUT_COLOR, 1.5f, 1.5f, 1.5f, 1.f) );

			// Set shader for cube & plane meshes
			CHECK( rprShapeSetMaterial(light, emissive) );
		}

		// Attach the light to the scene
		CHECK( rprSceneAttachShape(scene, light) );
	}

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "11.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(emissive)); emissive=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CHECK(rprObjectDelete(plane)); plane=nullptr;
	CHECK(rprObjectDelete(cube)); cube=nullptr;
	CHECK(rprObjectDelete(light)); light=nullptr;
	CHECK(rprObjectDelete(diffuse)); diffuse=nullptr;
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// 1) You could try to modify the emisive material
// 2) You could apply it to the plane