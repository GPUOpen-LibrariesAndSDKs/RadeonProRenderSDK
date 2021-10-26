/*****************************************************************************\
*
*  Module Name    Cutting Planes Demo
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
// This demos shows how to add Cutting planes (also called Clipping plane).
//

int main()
{
	//	for Debugging you can enable Radeon ProRender API trace
	//	set this before any RPR API calls
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

	// Create the scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 10, 10, 10,    0, 0, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// Create an Environment Light light
	rpr_light lightEnv = nullptr;
	rpr_image imgEnvLight = nullptr;
	{
		CHECK(rprContextCreateEnvironmentLight(context, &lightEnv));

		const std::string pathImageFile = "../../Resources/Textures/envLightImage.exr";
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFile.c_str(), &imgEnvLight); // import image use by the Env light
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFile << " not found.\n";
			return -1;
		}
		CHECK(status);

		CHECK(rprEnvironmentLightSetImage(lightEnv, imgEnvLight));
		CHECK(rprEnvironmentLightSetIntensityScale(lightEnv, 1.0f)); 
		CHECK(rprSceneAttachLight(scene, lightEnv));
	}



	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);
		RadeonProRender::matrix m = RadeonProRender::rotation_x(MY_PI);
		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m.m00));
	}


	// add a new shape representing the floor of the scene.
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

		RadeonProRender::matrix mat = RadeonProRender::scale(RadeonProRender::float3(0.5, 1, 0.5));
		CHECK(rprShapeSetTransform(plane, RPR_TRUE, &mat.m00));

		CHECK(rprSceneAttachShape(scene, plane));
	}

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	// create a DIFFUSE material for the Teapot
	//
	rpr_material_node diffuse1=nullptr;
	rpr_image img=nullptr;
	rpr_material_node imgSampler = nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		CHECK(rprContextCreateImageFromFile(context, "../../Resources/Textures/textest.png",&img));
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &imgSampler));
		CHECK(rprMaterialNodeSetInputImageDataByKey(imgSampler, RPR_MATERIAL_INPUT_DATA, img)); // Set image data
		CHECK(rprMaterialNodeSetInputNByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, imgSampler));
		CHECK(rprShapeSetMaterial(teapot01, diffuse1));
	}
	


	// create a DIFFUSE material for the Floor
	//
	rpr_material_node uv_scaled_node = NULL;
	rpr_material_node diffuse5 = nullptr;
	rpr_image image2 = nullptr;
	rpr_material_node materialImage2 = nullptr;
	rpr_material_node uv_node = NULL;
	{


		const std::string pathImageFileA = "../../Resources/Textures/amd.png";
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFileA.c_str(), &image2);
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFileA << " not found.\n";
			return -1;
		}
		CHECK(status);

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialImage2));
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialImage2, RPR_MATERIAL_INPUT_DATA, image2)); // Set image data

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse5));
		CHECK(rprMaterialNodeSetInputNByKey(diffuse5, RPR_MATERIAL_INPUT_COLOR, materialImage2)); // set image sampler as the color input of diffuse material

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node));
		CHECK(rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV));

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node));
		CHECK(rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL));
		CHECK(rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node));
		CHECK(rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 2.0f, 4.0f, 0, 0));

		CHECK(rprMaterialNodeSetInputNByKey(materialImage2, RPR_MATERIAL_INPUT_UV, uv_scaled_node));

		CHECK(rprShapeSetMaterial(plane, diffuse5));
	}
	

	// This is how we add cutting planes
	// we define with plane equation Ax + By + Cz + D = 0
	// The normal of the equation plane points toward the area that is kept.
	// in order to disable a cutting plane, just set A = B = C = D = 0.0
	CHECK(rprContextSetCuttingPlane(context,0,   0.0f, -1.0f, 0.0f, 2.5f )); // plane equation: -y + 2.5 = 0
	CHECK(rprContextSetCuttingPlane(context,1,   0.0f, 0.0f, -1.0f, 1.8f )); // plane equation: -z + 1.8 = 0


	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800 , 600 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Render the scene
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 60));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"27.png"));



	// Release the stuff we created
	CHECK(rprObjectDelete(img)); img=nullptr;
	CHECK(rprObjectDelete(imgSampler)); imgSampler=nullptr;
	CHECK(rprObjectDelete(uv_scaled_node)); uv_scaled_node=nullptr;
	CHECK(rprObjectDelete(diffuse5)); diffuse5=nullptr;
	CHECK(rprObjectDelete(image2)); image2=nullptr;
	CHECK(rprObjectDelete(materialImage2)); materialImage2=nullptr;
	CHECK(rprObjectDelete(uv_node)); uv_node=nullptr;
	CHECK(rprObjectDelete(diffuse1)); diffuse1=nullptr;
	CHECK(rprObjectDelete(lightEnv)); lightEnv=nullptr;
	CHECK(rprObjectDelete(imgEnvLight)); imgEnvLight=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(teapot01)); teapot01=nullptr;
	CHECK(rprObjectDelete(plane)); plane=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}




