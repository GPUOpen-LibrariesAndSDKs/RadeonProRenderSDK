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


	rpr_material_system matsys = nullptr;
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
	rpr_shape instance = nullptr;
	{
		rprContextCreateInstance( context, cube, &instance );

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(2, 1, 0));

		// Set the transform 
		CHECK( rprShapeSetTransform(instance, RPR_TRUE, &m.m00) );

		CHECK( rprSceneAttachShape(scene, instance) );
	}
	// Create env light
	rpr_light light = nullptr;
	rpr_image img = nullptr;
	{
		CHECK( rprContextCreateEnvironmentLight(context, &light) );

		const std::string pathImageFile = "../../Resources/Textures/envLightImage.exr";
		CHECK( rprContextCreateImageFromFile(context, pathImageFile.c_str(), &img) );
		if ( status == RPR_ERROR_IO_ERROR )
		{
			std::cout << "Error : " << pathImageFile << " not found.\n";
			return -1;
		}

		// Set an image for the light to take the radiance values from
		CHECK( rprEnvironmentLightSetImage(light, img) );

		// Set IBL as a background for the scene
		CHECK( rprSceneAttachLight(scene, light) );
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

		CHECK( rprShapeSetMaterial(instance, diffuse) );
	}


	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc;
	desc.fb_width = 800;
	desc.fb_height = 600;

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer frame_buffer;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );

	// Clear framebuffer to black color
	CHECK( rprFrameBufferClear(frame_buffer) );

	// Set framebuffer for the context
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );

	///////// Tutorial Material Layer //////////

	rpr_material_node base = nullptr;
	status = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &base);
	CHECK(status);

	rpr_material_node top = nullptr;
	status = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_MICROFACET, &top);
	CHECK(status);

	// Diffuse color
	status = rprMaterialNodeSetInputFByKey(base, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.25f, 0.f, 1.f);
	CHECK(status);

	// Specular color
	status = rprMaterialNodeSetInputFByKey(top, RPR_MATERIAL_INPUT_COLOR, 1.f, 1.f, 1.f, 1.f);
	CHECK(status);

	// Roughness
	status = rprMaterialNodeSetInputFByKey(top, RPR_MATERIAL_INPUT_ROUGHNESS, 0.05f, 0.f, 0.f, 1.f);
	CHECK(status);

	// Create layered shader
	//layered = create_layered_shader(context, matsys, base, top, 1.5f);
	rpr_material_node layered = nullptr;
	rpr_material_node fresnel = nullptr;
	{
		// Create a node
		status = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_BLEND, &layered);
		CHECK(status);

		// Set shader for base layer
		status = rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR0, base);
		CHECK(status);

		// Set shader for top layer
		status = rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR1, top);
		CHECK(status);

		status = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_FRESNEL, &fresnel); CHECK(status);
		status = rprMaterialNodeSetInputFByKey(fresnel, RPR_MATERIAL_INPUT_IOR, 1.5f,0.0f,0.0f,0.0f ); CHECK(status);
	
		// Set index of refraction for base layer
		status = rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_WEIGHT, fresnel);
		CHECK(status);
	}

	CHECK(rprShapeSetMaterial(plane, layered));
	

	// Progressively render an image
	for (int i = 0; i < NUM_ITERATIONS; ++i)
	{
		CHECK( rprContextRender(context) );
	}

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK( rprFrameBufferSaveToFile(frame_buffer, "21.png") );

	// Release the stuff we created
	CHECK(rprObjectDelete(base));base=nullptr;
	CHECK(rprObjectDelete(img));img=nullptr;
	CHECK(rprObjectDelete(fresnel));fresnel=nullptr;
	CHECK(rprObjectDelete(top));top=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(plane));plane=nullptr;
	CHECK(rprObjectDelete(cube));cube=nullptr;
	CHECK(rprObjectDelete(instance));instance=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	CHECK(rprObjectDelete(layered));layered=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

// Things to try in this tutorial:
// 1) Try to play with node maybe RPR_MATERIAL_NODE_REFLECTION, RPR_MATERIAL_NODE_REFRACTION, and other nodes
// 2) You may notice that it's not bouncing, you can raise recursion this way rprContextSetParameterByKey1u(context, "maxRecursion", 10) 