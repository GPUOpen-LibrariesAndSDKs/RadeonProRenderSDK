/*****************************************************************************\
*
*  Module Name    Camera Depth Of Field and Adaptive Sampling Demo
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
#include <chrono>

//
// This demo shows how to configure camera Depth of Field. This effect makes background and/or foreground blurry.
// 
// This demo also shows how to use Adaptive Sampling (AS).
// AS can be used instead of "Classic" rendering for faster image convergence.
// - In Classic rendering ( used in most of the demos ) we just define a number of iterations ( also called SPP ) with RPR_CONTEXT_ITERATIONS and call rprContextRender.
//   The higher RPR_CONTEXT_ITERATIONS the "more converged" ( less noisy ) the result will be. If image is still too noisy, you can call rprContextRender again, and continue the process until you have the desired quality.
// - For AS you decide the level of quality that you want ( with RPR_CONTEXT_ADAPTIVE_SAMPLING_THRESHOLD ), then the framebuffer is subdivided into small tiles and when a tile reach the level of quality, it stops to be computed. 
//   The number of active tiles is get with RPR_CONTEXT_ACTIVE_PIXEL_COUNT: when this reach 0 this means the rendering is finished.
//
// Note that AS is being used in this Depth of Field demo but can be used in any other rendering use-cases. There is no particular
// reason we choose to implemented Adaptive Sampling in this specific demo. 
// Also there is no rendering difference between AS and Classic except the noise level for the same rendering time.


// If RENDER_USING_ADAPTIVE_SAMPLING == 1 rendering will be done with Adaptive Sampling, otherwise Classic rendering will be done.
#define RENDER_USING_ADAPTIVE_SAMPLING 1

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
	RadeonProRender::float3 eyePos(4, 4.0, 15);
	CHECK( rprCameraLookAt(camera, eyePos.x,eyePos.y,eyePos.z,    1.5, 0, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 0.8f) );

	// Define the teapots list used in the scene
	struct TEAPOT_DEF
	{
		TEAPOT_DEF(float x_, float z_, float rot_, float r_, float g_, float b_)
		{
			x = x_;
			z = z_;
			rot = rot_;
			r = r_/255.0f;
			g = g_/255.0f;
			b = b_/255.0f;
			shape = nullptr;
			mat = nullptr;
		}

		float x;
		float z;
		float rot;
		float r;
		float g;
		float b;
		rpr_shape shape;
		rpr_material_node mat;
	};
	std::vector<TEAPOT_DEF> posList;
	posList.push_back(TEAPOT_DEF(5.0f , 2.0f,  1.6f   , 122,63,0)); // brown
	posList.push_back(TEAPOT_DEF(-5.0f , 3.0f,  5.6f  ,122 , 0 , 14));
	posList.push_back(TEAPOT_DEF(0.0f , -3.0f,  3.2f   ,  119 , 0 ,  93  ));
	posList.push_back(TEAPOT_DEF(1.0f , +3.0f,  1.2f ,  7  ,  0 ,  119));
	posList.push_back(TEAPOT_DEF(3.0f , +9.0f,  -1.7f   ,0  , 59 , 119));
	posList.push_back(TEAPOT_DEF(-6.0f , +12.0f,  2.2f  , 0 , 119 ,  99));
	posList.push_back(TEAPOT_DEF(9.0f , -6.0f,  4.8f    ,  0 ,  119 ,  1)); // green
	posList.push_back(TEAPOT_DEF(9.0f , 7.0f,  2.5f   ,  219 ,  170 ,  0)); // yellow
	posList.push_back(TEAPOT_DEF(-9.0f , -7.0f,  5.8f   , 112  , 216 , 202));
	

	// create teapots
	int i=0;
	for(const auto iShape : posList)
	{
		rpr_shape teapot01 = nullptr;

		if ( i == 0 )
		{
			// create from OBJ for the first teapot
			teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",scene,context);
		}
		else
		{
			// other teapots will be instances of the first one.
			CHECK( rprContextCreateInstance(context,posList[0].shape,&teapot01));
			CHECK( rprSceneAttachShape(scene, teapot01));
		}

		// random transforms of teapot on the floor :

		RadeonProRender::matrix m;
		
		if ( i % 4 == 0 )
		m =  RadeonProRender::translation(RadeonProRender::float3( iShape.x, 0.0f ,iShape.z)) *  RadeonProRender::rotation_y(iShape.rot) 
			* RadeonProRender::rotation_x(MY_PI);

		if ( i % 4 == 1 )
		m = RadeonProRender::translation(RadeonProRender::float3( iShape.x, 0.0f ,iShape.z)) *  RadeonProRender::rotation_y(iShape.rot) 
				*  RadeonProRender::translation(RadeonProRender::float3(0,2.65,0)) 
				*  RadeonProRender::rotation_x(MY_PI+1.9f)
				*  RadeonProRender::rotation_y(0.45);
		

		if ( i % 4 == 2 )
		m = RadeonProRender::translation(RadeonProRender::float3( iShape.x, 0.0f ,iShape.z)) *  RadeonProRender::rotation_y(iShape.rot) 
			*  RadeonProRender::translation(RadeonProRender::float3(0,2.65,0)) 
			*  RadeonProRender::rotation_x(MY_PI+1.9f)
			*  RadeonProRender::rotation_y(-0.57);
		

		if ( i % 4 == 3 )
		m =    RadeonProRender::translation(RadeonProRender::float3( iShape.x, 0.0f ,iShape.z)) *  RadeonProRender::rotation_y(iShape.rot) 
									*  RadeonProRender::translation(RadeonProRender::float3(0,3.38,0)) 
									*  RadeonProRender::rotation_x(+0.42f)
									*  RadeonProRender::rotation_z(-0.20f)
										;

		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m.m00));

		posList[i].shape = teapot01;

		i++;
	}


	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 1.0f, 1.0f)  );


	// create a material for the Teapots
	//
	for(auto& ishape : posList)
	{
		rpr_material_node material = nullptr;

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_UBERV2, &material));

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR, ishape.r, ishape.g, ishape.b, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT, 1.f, 1.f, 1.f, 1.f));

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, 0.5f, 0.5f, 0.5f, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT, 0.9f, 0.9f, 0.9f, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, 0.1f, 0.f, 0.f, 0.f));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE, RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_METALNESS, 0.f, 0.f, 0.f, 1.f));

		CHECK(rprShapeSetMaterial(ishape.shape, material));

		ishape.mat = material;
	}
	

	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800 , 600 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	// for Adaptive Sampling, we always need a Variance AOV.
	// This AOV keep track of active pixels ( pixels where color has changed between two rendering iterations )
	#if RENDER_USING_ADAPTIVE_SAMPLING
	rpr_framebuffer frame_buffer_variance = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_variance));
	CHECK( rprContextSetAOV(context, RPR_AOV_VARIANCE, frame_buffer_variance));
	#endif

	// setting the Focal Length modifies the "Field of View" of the camera.
	CHECK( rprCameraSetFocalLength(camera, 25.0f));

	// This is the most important part of this Depth of Field Demo :  here we change the focus.
	// Note that FocusDistance should be the distance between the eye and the point we focus ( in this case, the top of the blue teapot )
	// Then we adjust FStop. This parameters set the blur effect we want to give to the non-focused parts.  1/~0.0 = FLT_MAX is the default value, meaning no blur effect.
	//                       The more we increase the denominator the more blur effect we have.
	RadeonProRender::float3 focusPoint(3, 3.6, 9);
	RadeonProRender::float3 eyeToFocus = focusPoint - eyePos;
	CHECK( rprCameraSetFocusDistance(camera,  std::sqrt(eyeToFocus.sqnorm())     ));
	CHECK( rprCameraSetFStop(camera, 1.0f/11.0f));


	// This Demo rendering is a good opportunity to show the ApertureBlades feature.
	// With this function you can modify the shape of lens flares.
	// With 5, you will notice pentagon artifacts.
	// ( You can try to comment-out this line and see the difference - it should switch to Hexagon, default value of the renderer )
	CHECK( rprCameraSetApertureBlades(camera, 5));

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// By default, there is no radiance clamp ( RPR_CONTEXT_RADIANCE_CLAMP = FLT_MAX )
	// On this kind of scene, adding some radiance clamping helps to reduce the fireflies and to have a faster image convergence
	// ( You can try to comment-out this line and see the difference )
	CHECK( rprContextSetParameterByKey1f(context,RPR_CONTEXT_RADIANCE_CLAMP, 2.0f ) );

	//
	// Render the scene
	//

	std::chrono::high_resolution_clock timer;
	auto timeStart = timer.now();

	#if RENDER_USING_ADAPTIVE_SAMPLING

	// Adaptive Sampling works with dividing the framebuffer into tiles - this is tile size in pixels - recommended : [4, 16]
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_ADAPTIVE_SAMPLING_TILE_SIZE, 8)); 

	// minimum of iterations a pixel must do before activating Adaptive Sampling - recommended to set 10 or more
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_ADAPTIVE_SAMPLING_MIN_SPP, 10));

	// between 0 and 1 -  lower value means better final image 
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_ADAPTIVE_SAMPLING_THRESHOLD, 0.008f)); 

	// iteration between each  RPR_CONTEXT_ACTIVE_PIXEL_COUNT  test.
	// it's better to keep this number high enough.  ( =1 would kill performance )...  but not too high otherwise you lose the advantage of Adaptive Sampling.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 100));

	// call rprContextRender until all pixels are inactive
	for(int iActivePxlIteration=0;;iActivePxlIteration++)
	{
		// adds more SPP to the current framebuffer
		CHECK( rprContextRender(context) );

		// get number of active pixels.
		rpr_uint activePixelCount = 0;
		CHECK( rprContextGetInfo(context, RPR_CONTEXT_ACTIVE_PIXEL_COUNT, sizeof(activePixelCount), &activePixelCount, 0));
		std::cout<<"Iteration " << iActivePxlIteration <<  " - " << activePixelCount << " active pixels remaining..." << std::endl;

		// end when no more pixel have changed.
		if ( activePixelCount == 0 )
			break;
	}

	#else

	// Classic rendering
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 4000));
	CHECK( rprContextRender(context) );

	#endif

	auto timeEnd = timer.now();
	auto timeRendering = std::chrono::duration_cast<std::chrono::seconds>(timeEnd - timeStart).count();
	std::cout<<"Rendering took " << timeRendering << " seconds." << std::endl;
	

	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"17.png"));


	//
	// Release the stuff we created
	//

	#if RENDER_USING_ADAPTIVE_SAMPLING
	CHECK(rprObjectDelete(frame_buffer_variance)); frame_buffer_variance=nullptr;
	#endif

	for(auto& ishape : posList)
	{
		CHECK(rprObjectDelete(ishape.mat)); ishape.mat=nullptr;
		CHECK(rprObjectDelete(ishape.shape));  ishape.shape=nullptr;
	}

	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}




