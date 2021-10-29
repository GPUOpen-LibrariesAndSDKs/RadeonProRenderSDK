/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
// Shows how to export an RPR scene as RPRS files ( native RPR file format ) or GLTF ( Khronos Group ).
//
//


#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h" //For Export to RPRS ( native RPR file format )
#include "ProRenderGLTF.h" //For Export to GLTF
#include "../common/common.h"

#include <cassert>
#include <iostream>

RPRGarbageCollector g_gc;


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

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK(rprContextCreateMaterialSystem(context, 0, &matsys));

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK(rprContextCreateScene(context, &scene));

	// Set scene to render for the context
	CHECK(rprContextSetScene(context, scene));

	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 0.7f, 0.7f)  );
	
	// create env light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 0.8f)  );

	// Create camera
	rpr_camera camera = nullptr;
	{
		CHECK(rprContextCreateCamera(context, &camera));
		CHECK(rprCameraLookAt(camera, 10, 6, 23,   0, 1, 0,   0, 1, 0));
		CHECK(rprCameraSetFocalLength(camera, 75.f));
		CHECK(rprSceneSetCamera(scene, camera));
	}


	// Create framebuffer
	rpr_framebuffer_desc desc = { 800,600 };
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer
	CHECK(rprFrameBufferClear(frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));


	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);
		g_gc.GCAdd(teapot01);

		RadeonProRender::matrix m0 = RadeonProRender::rotation_x(MY_PI);
		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m0.m00));
	}


	// Create Material
	{
		rpr_material_node uberMat = nullptr;
		CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat));
		g_gc.GCAdd(uberMat);
		CHECK(rprObjectSetName(uberMat,"Uber_0"));

		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,0.501961f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,0.500000f,0.500000f,0.500000f,0.500000f));

		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,0.490196f,0.490196f,0.490196f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,0.990000f,0.990000f,0.990000f,0.990000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,0.008000f,0.008000f,0.008000f,0.008000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.460000f,1.460000f,1.460000f,1.460000f));

		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_COLOR,0.490196f,0.490196f,0.490196f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS,0.008000f,0.008000f,0.008000f,0.008000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_IOR,1.460000f,1.460000f,1.460000f,1.460000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR,0.0f,0.0f,0.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS,0.0f,0.0f,0.0f,0.0f));

		CHECK(rprShapeSetMaterial(teapot01, uberMat));
	}

	// set display gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );


	///////// Mesh Export to RPRS ( native RPR file format ) //////////
	CHECK(rprsxExport("cube_floor.rprs", context, nullptr, scene, 0, 0, 0, 0, 0, 0, 0, nullptr));

	///////// Mesh Export to GLTF //////////
	CHECK(rprExportToGLTF("cube_floor.gltf", context, nullptr, &scene, 1, 0, nullptr));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "60.png"));


	// Release the stuff we created
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	g_gc.GCClean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


