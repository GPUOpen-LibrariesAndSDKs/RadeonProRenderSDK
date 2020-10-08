/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright 2011 - 2017 Advanced Micro Devices, Inc. (unpublished)
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
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0, NULL, NULL, &context) );

	// Set active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );


	rpr_material_system matsys=nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

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
		// Set light radiant power in Watts
		CHECK(rprPointLightSetRadiantPower3f(light, 100, 100, 100));
		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}
	// Create camera
	rpr_camera camera=nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		// Camera position is (5,5,20)
		// Camera aimed at (0,0,0)
		// Camera up vector is (0,1,0)
		CHECK( rprCameraLookAt(camera, 0, 5, 80, 0, 1, 0, 0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );


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

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 0, 0));
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));
	}
	// Create plane mesh
	rpr_shape plane=nullptr;
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


	// Create simple diffuse shader
	rpr_material_node diffuse=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse));

		// Set diffuse color parameter to gray
		CHECK(rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f));

		// Set shader for cube & plane meshes
		CHECK(rprShapeSetMaterial(cube, diffuse));

		CHECK(rprShapeSetMaterial(plane, diffuse));
	}

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));


	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	///////// Framebuffer Access Tutorial //////////

	size_t size;
	CHECK(rprFrameBufferGetInfo(frame_buffer_resolved, RPR_FRAMEBUFFER_DATA, 0, NULL, &size));
	float* buffer = new float[size / sizeof(float)];
	CHECK(rprFrameBufferGetInfo(frame_buffer_resolved, RPR_FRAMEBUFFER_DATA, size, buffer, 0));

	//Apply this buffer as a texture

	rpr_material_node diffuse1=nullptr;
	rpr_material_node tex=nullptr;
	rpr_image img1=nullptr;
	{
		rpr_image_format format;
		format.num_components = 4;
		format.type = RPR_COMPONENT_TYPE_FLOAT32;
		rpr_image_desc desc;
		desc.image_width = 800;
		desc.image_height = 600;
		desc.image_row_pitch = 0;
		desc.image_slice_pitch = 0;
		desc.image_depth = 1;
	
		CHECK(rprContextCreateImage(context, format, &desc, buffer, &img1));
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &tex));
		// Set image data
		CHECK(rprMaterialNodeSetInputImageDataByKey(tex, RPR_MATERIAL_INPUT_DATA, img1));
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		// Set diffuse color parameter to gray
		CHECK(rprMaterialNodeSetInputNByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, tex));
		CHECK(rprShapeSetMaterial(plane, diffuse1));
	}
	
	//Remove the cube and turn plane to dislay the texture in front of us like a screenshot
	CHECK(rprSceneDetachShape(scene, cube));
	RadeonProRender::matrix m = RadeonProRender::rotation(RadeonProRender::float3(1, 0, 0), -PI / 2.0f);
	CHECK(rprShapeSetTransform(plane, RPR_TRUE, &m.m00));
	//Clear the buffer to render again
	rprFrameBufferClear(frame_buffer);

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));
	std::cout << "Rendering finished.\n";

	//delete buffer;

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved, "31.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(tex));tex=nullptr;
	CHECK(rprObjectDelete(img1));img1=nullptr;
	CHECK(rprObjectDelete(diffuse1));diffuse1=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(plane));plane=nullptr;
	CHECK(rprObjectDelete(cube));cube=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// 1) Try to access other AOV