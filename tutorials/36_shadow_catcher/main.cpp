/*****************************************************************************\
*
*  Module Name    Shadow Catcher Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2022 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "../common/common.h"
#include <cassert>
#include <iostream>

//
// Demo of the Shadow Catcher. If a shape has this feature activated, it will "catch" the shadow. This shadow quantity can be rendered on a dedicated AOV.
//

// simple garbage collector for RPR objects.
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
	g_gc.GCAdd(camera);
	CHECK( rprCameraLookAt(camera, 10, 10, 10,    0, -1, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	
	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 1.0f) );


	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);
		g_gc.GCAdd(teapot01);
		RadeonProRender::matrix m = RadeonProRender::rotation_y(MY_PI/2.0f) * RadeonProRender::rotation_x(MY_PI);
		CHECK( rprShapeSetTransform(teapot01, RPR_TRUE, &m.m00));
	}

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	// create the shadow catcher plane shape.
	// This plane will be used to "catch" the shadow quantity.
	{
		rpr_shape plane = nullptr;
		rpr_material_node diffuse1=nullptr;

		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &plane));
		g_gc.GCAdd(plane);

		RadeonProRender::matrix mat = RadeonProRender::scale(RadeonProRender::float3(0.5f, 1.0f, 0.5f));
		CHECK(rprShapeSetTransform(plane, RPR_TRUE, &mat.m00));

		CHECK(rprSceneAttachShape(scene, plane));

		// use a simple white diffuse material
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		g_gc.GCAdd(diffuse1);
		CHECK( rprMaterialNodeSetInputFByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, 1.0, 1.0, 1.0, 1.0f));
		CHECK( rprShapeSetMaterial(plane, diffuse1));

		// Enable the shadown catcher feature on this plane.
		CHECK( rprShapeSetShadowCatcher(plane,true) );
	}


	// create a DIFFUSE material for the Teapot
	//
	{
		rpr_material_node diffuse1=nullptr;
		rpr_image img=nullptr;
		rpr_material_node imgSampler = nullptr;

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		g_gc.GCAdd(diffuse1);
		CHECK( rprContextCreateImageFromFile(context, "../../Resources/Textures/textest.png",&img));
		g_gc.GCAdd(img);

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &imgSampler));
		g_gc.GCAdd(imgSampler);
		CHECK( rprMaterialNodeSetInputImageDataByKey(imgSampler, RPR_MATERIAL_INPUT_DATA, img)); // Set image data
		CHECK( rprMaterialNodeSetInputNByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, imgSampler));
		CHECK( rprShapeSetMaterial(teapot01, diffuse1));
	}
	

	// Create the Color framebuffers
	rpr_framebuffer_desc desc = { 800 , 600 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	g_gc.GCAdd(frame_buffer);
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	g_gc.GCAdd(frame_buffer_resolved);
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );

	// Create the frambuffers dedicated to the shadow catching.
	rpr_framebuffer frame_buffer_shadowCatcher_unresolved = 0;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_shadowCatcher_unresolved) );
	g_gc.GCAdd(frame_buffer_shadowCatcher_unresolved);
	rpr_framebuffer frame_buffer_shadowCatcher_resolved = 0;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_shadowCatcher_resolved) );
	g_gc.GCAdd(frame_buffer_shadowCatcher_resolved);
	CHECK( rprContextSetAOV(context, RPR_AOV_SHADOW_CATCHER, frame_buffer_shadowCatcher_unresolved) );


	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Render the scene
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 200));
	CHECK( rprContextRender(context) );
	
	// resolve and render the color framebuffer
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"36_0_c.png"));

	// resolve and render the shadow-catcher framebuffer
	// Note that we use 'true' for the last argument of rprContextResolveFrameBuffer. This is because we don't want to apply gamma on the shadow catcher AOV. 
	// That would not make sense as this AOV reprents a "shadow quantity", not a real color.
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer_shadowCatcher_unresolved,frame_buffer_shadowCatcher_resolved, true) );
	CHECK( rprFrameBufferSaveToFile(frame_buffer_shadowCatcher_resolved,"36_0_s.png"));


	/////////////////////
	//
	// demo of the RPR_CONTEXT_SHADOW_CATCHER_BAKING parameter.
	// If we turn it to 0, the shadow catcher plane will be invisible in the color framebuffer.
	// this parameter doesn't affect the Shadow Catcher AOV.
	//

	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_SHADOW_CATCHER_BAKING,0));

	// always clear previous AOVs before starting a new rendering.
	CHECK( rprFrameBufferClear(frame_buffer)); 
	CHECK( rprFrameBufferClear(frame_buffer_shadowCatcher_unresolved));

	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"36_1_c.png"));


	/////////////////////
	//
	// Release the stuff we created

	g_gc.GCClean();
	CHECK( rprObjectDelete(scene)); scene=nullptr;
	CHECK( rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK( rprObjectDelete(context)); context=nullptr;
	return 0;
}




