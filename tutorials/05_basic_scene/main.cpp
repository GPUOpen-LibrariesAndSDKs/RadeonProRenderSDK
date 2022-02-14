/*****************************************************************************\
*
*  Module Name    Basic Scene Demo
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
// This demo is a good starting point to learn all the basic features of RPR.
// You'll learn how to manage  geometry, light, camera, framebuffer, materials for simple cubes renderings.
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


	// Create a scene. A scene is a container of nodes we want to render.
	// RPR can manage multiple scenes per context.
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.


	// Create the camera
	rpr_camera camera = nullptr;
	{
		// create the camera
		CHECK( rprContextCreateCamera(context, &camera) );

		// camera position in world space:
		CHECK( rprCameraLookAt(camera, 0, 5, 10,    0, 0, 0,    0, 1, 0) );

		// attach the camera to the scene.
		// meaning this camera will be used for rendering.
		// ( a scene may have multiple cameras )
		CHECK( rprSceneSetCamera(scene, camera) );
	}


	// Create cube mesh
	rpr_shape cube = nullptr;
	{
		// geometry is defined at the creation time.
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube));

		// attach the cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1, 0));
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
	}


	// Create a simple point light
	rpr_light light=nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Set transform for the light
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(2, 10, 2));
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light radiant power
		CHECK(rprPointLightSetRadiantPower3f(light, 150, 150, 150));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}



	// Create framebuffer to store rendering result
	// For basic rendering We need 2 framebuffers:
	// 'frame_buffer' will be used to store the Color AOV.
	// 'frame_buffer_resolved' is the resolution of 'frame_buffer', meaning that 'frame_buffer' is not supposed to be displayed and should only be used internally.
	//                         however 'frame_buffer_resolved' is the correct rendering that can be displayed on the screen. 
	rpr_framebuffer_desc desc = { 800 , 600 }; // resolution in pixels
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; // format: 4 component 32-bit float value each
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) ); // attach 'frame_buffer' to the Color AOV ( this is the main AOV for final rendering )

	// define number of iterations.
	// The higher iterations we use, the better the quality will be.
	// Note that having RPR_CONTEXT_ITERATIONS=N  is pretty much the same thing that calling rprContextRender N times.
	//      However, performances are better when using the RPR_CONTEXT_ITERATIONS.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 60));

	// Start the rendering. 
	// Note that this call is synchronous ( as most of the RPR API ).
	// There are ways to call rprContextRender in a separate thread but this won't be done in this tutorial.
	CHECK( rprContextRender(context) );

	// 'frame_buffer' is not supposed to be used for rendering, we need to process it with rprContextResolveFrameBuffer.
	// This function transforms the raw 'frame_buffer' into a new 'frame_buffer_resolved' that can be displayed on screen as final rendering.
	// The 'noDisplayGamma'=false argument means we want to use display gamma (defined by RPR_CONTEXT_DISPLAY_GAMMA) which makes sense in this case as 'frame_buffer' represents COLOR.
	// If the framebuffer doesn't represent Color ( for example: Normals, Depth, UVs ... ) then 'noDisplayGamma' should be set to true. 
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	// save the rendering to an image file.
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"05_00.png"));

	std::cout << "rendering 00 finished." << std::endl;

	//
	// At this point, we have our first RPR rendering, saved in the 05_00.png file.
	// We are going to complexify this scene in order to demonstrate more RPR features.
	//


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

		// Add plane into the scene
		CHECK(rprSceneAttachShape(scene, plane));
	}


	// use the instance feature to create another cube base of the first one.
	rpr_shape cube_instance=nullptr;
	{
		rprContextCreateInstance(context, cube, &cube_instance);

		// Create a transform: 
		RadeonProRender::matrix m = 
			RadeonProRender::translation(RadeonProRender::float3(2, 1, -3)) *
			RadeonProRender::rotation_y(0.5);

		// Set the transform 
		CHECK(rprShapeSetTransform(cube_instance, RPR_TRUE, &m.m00));

		CHECK(rprSceneAttachShape(scene, cube_instance));
	}


	// create materials 
	//

	// Before creation materials, we need a Material System.
	// This object is just the containers for all materials of the scene.
	// In most of the cases, you will need 1 single Material System per RPR context.
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// create a DIFFUSE material for the Cube
	rpr_material_node diffuse1=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));

		// set the color
		CHECK(rprMaterialNodeSetInputFByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, 0.6f, 0.4f, 1.0f, 0.0f));

		// set it to the cube
		CHECK(rprShapeSetMaterial(cube, diffuse1));
	}

	// do the same for the instance and floor
	rpr_material_node diffuse2=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse2));
		CHECK(rprMaterialNodeSetInputFByKey(diffuse2, RPR_MATERIAL_INPUT_COLOR, 1.0, 0.5, 0.0, 0.0));
		CHECK(rprShapeSetMaterial(cube_instance, diffuse2));
	}
	rpr_material_node diffuse3=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse3));
		CHECK(rprMaterialNodeSetInputFByKey(diffuse3, RPR_MATERIAL_INPUT_COLOR, 0.1f, 0.8f, 1.0f, 0.0f));
		CHECK(rprShapeSetMaterial(plane, diffuse3));
	}


	// Let's render our current scene
	//

	// Note that when the scene has been changed, it's important to clear the framebuffer.
	// rprContextRender always accumulates the rendering to 'frame_buffer'.
	// meaning you will have a blending of the rendering and the previous one.
	CHECK( rprFrameBufferClear(frame_buffer) );

	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"05_01.png"));


	// We will now learn how to add images.
	//

	rpr_material_node diffuse4 = nullptr;
	rpr_image image1 = nullptr;
	rpr_material_node materialImage1 = nullptr;
	{
		
		const std::string pathImageFileA = "../../Resources/Textures/lead_rusted_Base_Color.jpg";
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFileA.c_str(), &image1);
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFileA << " not found.\n";
			return -1;
		}
		CHECK(status);

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialImage1));
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialImage1, RPR_MATERIAL_INPUT_DATA, image1)); // Set image data

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse4));
		CHECK(rprMaterialNodeSetInputNByKey(diffuse4, RPR_MATERIAL_INPUT_COLOR, materialImage1)); // set image sampler as the color input of diffuse material

		CHECK(rprShapeSetMaterial(cube, diffuse4));
	}


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

		// create a Lookup material and define it as a "UV Lookup" meaning the output of this material is the UV from the shape.
		// Lookup nodes are useful to create procedural materials.
		// UV-Lookup are often used to scale textures on shapes.
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node));
		CHECK(rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV));

		// adjust the texture scale by multiplying the the UV by a constant
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node));
		CHECK(rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL));
		CHECK(rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node));
		CHECK(rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 6.0f, 12.0f, 0, 0));

		// apply this modified UV to the image material.
		CHECK(rprMaterialNodeSetInputNByKey(materialImage2, RPR_MATERIAL_INPUT_UV, uv_scaled_node));

		CHECK(rprShapeSetMaterial(plane, diffuse5));
	}

	// create a simple reflection material and apply it on the cube_instance
	rpr_material_node reflection1=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_REFLECTION, &reflection1));
		CHECK(rprMaterialNodeSetInputFByKey(reflection1, RPR_MATERIAL_INPUT_COLOR, 1.0, 1.0, 1.0, 0.0));
		CHECK(rprShapeSetMaterial(cube_instance, reflection1));
	}

	// remove the point light: we are going to replace it by an Environment Light
	CHECK(rprSceneDetachLight(scene, light));

	// Create an Environment Light light
	rpr_light lightEnv = nullptr;
	rpr_image imgEnvLight = nullptr;
	{
		CHECK(rprContextCreateEnvironmentLight(context, &lightEnv));

		const std::string pathImageFile = "../../Resources/Textures/turning_area_4k.hdr";
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFile.c_str(), &imgEnvLight); // import image use by the Env light
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFile << " not found.\n";
			return -1;
		}
		CHECK(status);

		// Set an image for the Env light to take the radiance values from
		CHECK(rprEnvironmentLightSetImage(lightEnv, imgEnvLight));

		// adjust the intensity of the Env light
		CHECK(rprEnvironmentLightSetIntensityScale(lightEnv, 0.8f)); 

		//optional: env light can be rotated :
		//RadeonProRender::matrix lightm = RadeonProRender::rotation_y(MY_PI/2.0f);
		//CHECK( rprLightSetTransform(lightEnv, true, &lightm.m00));

		// Set Env light as a background for the scene
		CHECK(rprSceneAttachLight(scene, lightEnv));
	}

	// move camera 
	CHECK( rprCameraLookAt(camera,  0, 4, 10,    0, 1, 0,    0, 1, 0) );

	// modify display gamma. In most of the cases, display gamma should be around 2.2.
	// This makes image brightness looking better on majority of monitors.
	// Gamma is applied during the "rprContextResolveFrameBuffer" call.
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// render the current scene the same way we did for 05_01.png
	CHECK(rprFrameBufferClear(frame_buffer) );
	CHECK(rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"05_02.png"));


	// the scene can be dynamically animated by updating parameters
	//
	// example:

	// move camera 
	CHECK( rprCameraLookAt(camera, 0, 4, 9,    0, 0, 0,    0, 1, 0) );

	// move cube
	RadeonProRender::matrix m = 
		RadeonProRender::translation(RadeonProRender::float3(1, 1, -3)) *
		RadeonProRender::rotation_y(0.7f) *
		RadeonProRender::scale(RadeonProRender::float3(1.0, 1.0, 4.0));
	CHECK(rprShapeSetTransform(cube_instance, RPR_TRUE, &m.m00));

	// change scaling of the AMD logo on the floor
	CHECK(rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 10.0f, 20.0f, 0, 0));

	// replace the material on cuve by an emissive one.
	rpr_material_node emissive1=nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_EMISSIVE, &emissive1));
	CHECK(rprMaterialNodeSetInputFByKey(emissive1, RPR_MATERIAL_INPUT_COLOR, 6.0, 3.0, 0.0, 0.0));
	CHECK(rprShapeSetMaterial(cube, emissive1));

	// move cube
	RadeonProRender::matrix mCube = 
		RadeonProRender::translation(RadeonProRender::float3(-2.0f, 0.7f, 0.0f)) *
		RadeonProRender::rotation_y(0.0) *
		RadeonProRender::scale(RadeonProRender::float3(0.5, 0.5, 0.5));
	CHECK(rprShapeSetTransform(cube, RPR_TRUE, &mCube.m00));

	rprShapeSetVisibilityFlag(cube,RPR_SHAPE_VISIBILITY_SHADOW,false);

	// remove the Env light, and use the point light again.
	CHECK(rprSceneAttachLight(scene, light));
	CHECK(rprSceneDetachLight(scene, lightEnv));

	// slightly increase the iteration count for this scene.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 120));

	// render the current scene the same way we did for 05_01.png
	CHECK(rprFrameBufferClear(frame_buffer) );
	CHECK(rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"05_03.png"));


	// Release the stuff we created
	// Order of delete calls doesn't matter, as long as you delete the Context in last.
	//
	// In RPR, you can only rprObjectDelete an object when it's no longer used in the scene: 
	//  calling a rprContextRender with delete object will lead into unstable behaviour.
	//
	CHECK(rprObjectDelete(diffuse1)); diffuse1=nullptr;
	CHECK(rprObjectDelete(uv_node)); uv_node=nullptr;
	CHECK(rprObjectDelete(image2)); image2=nullptr;
	CHECK(rprObjectDelete(materialImage2)); materialImage2=nullptr;
	CHECK(rprObjectDelete(diffuse2)); diffuse2=nullptr;
	CHECK(rprObjectDelete(diffuse3)); diffuse3=nullptr;
	CHECK(rprObjectDelete(imgEnvLight)); imgEnvLight=nullptr;
	CHECK(rprObjectDelete(diffuse5)); diffuse5=nullptr;
	CHECK(rprObjectDelete(diffuse4)); diffuse4=nullptr;
	CHECK(rprObjectDelete(image1)); image1=nullptr;
	CHECK(rprObjectDelete(materialImage1)); materialImage1=nullptr;
	CHECK(rprObjectDelete(cube_instance)); cube_instance=nullptr;
	CHECK(rprObjectDelete(emissive1)); emissive1=nullptr;
	CHECK(rprObjectDelete(light)); light=nullptr;
	CHECK(rprObjectDelete(reflection1)); reflection1=nullptr;
	CHECK(rprObjectDelete(lightEnv)); lightEnv=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(uv_scaled_node)); uv_scaled_node=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(cube)); cube=nullptr;
	CHECK(rprObjectDelete(plane)); plane=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}


