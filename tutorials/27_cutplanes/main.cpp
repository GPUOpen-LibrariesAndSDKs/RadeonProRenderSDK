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

RPRGarbageCollector g_gc;

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
	

	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 1.0f) );



	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);
		RadeonProRender::matrix m = RadeonProRender::rotation_x(MY_PI);
		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m.m00));
	}

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 0.5f, 0.5f)  );


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
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 200));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"27.png"));



	// Release the stuff we created
	CHECK(rprObjectDelete(img)); img=nullptr;
	CHECK(rprObjectDelete(imgSampler)); imgSampler=nullptr;
	CHECK(rprObjectDelete(diffuse1)); diffuse1=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	CHECK(rprObjectDelete(teapot01)); teapot01=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}




