/*****************************************************************************\
*
*  Module Name    Transform Motion Demo
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
// This demo covers shape and camera matrix transform changed over time for a blur effect.
// Note that this is different compared to a Deformation motion blur ( illustrated in another Demo ) where we set each vertex individually over time.
// Here it's simpler: we just change the transform matrix.
// This demo also illustrates how we can export the blur with the RPR_AOV_VELOCITY AOV.
//


// convert a  pos/lookat/up  into a camera transform matrix
RadeonProRender::matrix CameraLookAtToMatrix( RadeonProRender::float3 pos ,  RadeonProRender::float3 at,  RadeonProRender::float3 up)
{
	RadeonProRender::float3 directionVector =  RadeonProRender::normalize(at - pos);
	RadeonProRender::float3 right = normalize(RadeonProRender::cross(directionVector,up));

	// Warning: For rprCameraSetMotionTransform, we need to make sure to have both 'right' and 'up2' correctly orthogonal to 'directionVector'
	//           otherwise it may result into bad blur rendering.
	RadeonProRender::float3 up2 = normalize(RadeonProRender::cross(right,directionVector));

	RadeonProRender::matrix m(
		right.x,            right.y,            right.z,            0.0,
		up2.x,              up2.y,              up2.z,               0.0,
		-directionVector.x, -directionVector.y, -directionVector.z, 0.0,
		pos.x,              pos.y,              pos.z,              1.0
	);
	return m;
}


RPRGarbageCollector g_gc;


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

	// create material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// Create the scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );
	CHECK( rprContextSetScene(context, scene) );

	// Create camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 10, 10, 10,   0, 1, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800 , 600 };
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer fb_color = nullptr;
	rpr_framebuffer fb_color_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &fb_color) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &fb_color_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, fb_color) );

	// Optional : add a Velocity AOV. this could be useful for debugging or post processing
	rpr_framebuffer fb_velocity = nullptr;
	rpr_framebuffer fb_velocity_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &fb_velocity) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &fb_velocity_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_VELOCITY, fb_velocity) );


	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);

		RadeonProRender::matrix m0 = RadeonProRender::rotation_x(MY_PI);
		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m0.m00));
	}

	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 1.0f, 1.0f)  );

	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 0.8f) );

	
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


	// First, Render scene without any motion
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) ); // set display gamma
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,200));
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,fb_color,fb_color_resolved,false));
	CHECK( rprContextResolveFrameBuffer(context,fb_velocity,fb_velocity_resolved,true));
	CHECK( rprFrameBufferSaveToFile(fb_color_resolved, "12_0.png") );
	std::cout << "Rendering 12_0 finished." << std::endl;


	// activate the motion by setting an exposure greater than 0.0
	CHECK(rprCameraSetExposure(camera,1.0));


	// Apply a rotation motion blur to the teapot
	// Note that rotation_x(MY_PI) is the transform at exposure=0 ( before motion blur ), and  rotation_y(0.5f)  is the real motion blur effect.
	{
		RadeonProRender::matrix m1 = RadeonProRender::rotation_y(0.1f) * RadeonProRender::rotation_x(MY_PI);
		CHECK( rprShapeSetMotionTransform(teapot01, RPR_TRUE, &m1.m00, 1)); // define the transform at  exposure=1.0
		CHECK( rprShapeSetMotionTransformCount(teapot01, 1)); // specify that the matrix at 'timeIndex'=1 must be used
	}

	// Render the scene
	CHECK( rprFrameBufferClear(fb_color) );
	CHECK( rprFrameBufferClear(fb_velocity) );
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,fb_color,fb_color_resolved,false));
	CHECK( rprContextResolveFrameBuffer(context,fb_velocity,fb_velocity_resolved,true));
	CHECK( rprFrameBufferSaveToFile(fb_color_resolved, "12_1.png") );
	CHECK( rprFrameBufferSaveToFile(fb_velocity_resolved, "12_1v.png") );
	std::cout << "Rendering 12_1 finished." << std::endl;

	
	// Same that previous rendering, but instead of a rotation we do a  (0.0f,0.0f,-0.3f)  blur translation of the teapot
	RadeonProRender::matrix m1 = RadeonProRender::translation(RadeonProRender::float3(0.0f,0.0f,-0.3f)) * RadeonProRender::rotation_x(MY_PI);
	CHECK( rprShapeSetMotionTransform(teapot01, RPR_TRUE, &m1.m00, 1));
	CHECK( rprShapeSetMotionTransformCount(teapot01, 1));
	CHECK( rprFrameBufferClear(fb_color) );
	CHECK( rprFrameBufferClear(fb_velocity) );
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,fb_color,fb_color_resolved,false));
	CHECK( rprContextResolveFrameBuffer(context,fb_velocity,fb_velocity_resolved,true));
	CHECK( rprFrameBufferSaveToFile(fb_color_resolved, "12_2.png") );
	CHECK( rprFrameBufferSaveToFile(fb_velocity_resolved, "12_2v.png") );
	std::cout << "Rendering 12_2 finished." << std::endl;

	// Now, we do a movement of the camera instead of shape. This is pretty much the same API.
	RadeonProRender::matrix camMat1 = CameraLookAtToMatrix( RadeonProRender::float3(9, 9, 9) , RadeonProRender::float3(0,1,0) , RadeonProRender::float3(0,1,0) );
	CHECK( rprCameraSetMotionTransform(camera, false, &camMat1.m00 ,1 )); // define the camera transform at  exposure=1.0
	CHECK( rprCameraSetMotionTransformCount(camera, 1)); // enable motion blur on camera
	CHECK( rprShapeSetMotionTransformCount(teapot01, 0)); // disable motion blur of teapot
	CHECK( rprFrameBufferClear(fb_color) );
	CHECK( rprFrameBufferClear(fb_velocity) );
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,fb_color,fb_color_resolved,false));
	CHECK( rprContextResolveFrameBuffer(context,fb_velocity,fb_velocity_resolved,true));
	CHECK( rprFrameBufferSaveToFile(fb_color_resolved, "12_3.png") );
	CHECK( rprFrameBufferSaveToFile(fb_velocity_resolved, "12_3v.png") );
	std::cout << "Rendering 12_3 finished." << std::endl;



	// Release the stuff we created
	// 
	CHECK(rprObjectDelete(img)); img=nullptr;
	CHECK(rprObjectDelete(imgSampler)); imgSampler=nullptr;
	CHECK(rprObjectDelete(diffuse1)); diffuse1=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(fb_color)); fb_color=nullptr;
	CHECK(rprObjectDelete(fb_color_resolved)); fb_color_resolved=nullptr;
	CHECK(rprObjectDelete(fb_velocity)); fb_velocity=nullptr;
	CHECK(rprObjectDelete(fb_velocity_resolved)); fb_velocity_resolved=nullptr;
	CHECK(rprObjectDelete(teapot01)); teapot01=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


