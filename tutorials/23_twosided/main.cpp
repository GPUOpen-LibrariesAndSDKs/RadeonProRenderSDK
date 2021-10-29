/*****************************************************************************\
*
*  Module Name    Two Sided Material Demo
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
// This demo illustrates how to define different materials on the front and back face of mesh faces.
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
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set the active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "RPR Context creation succeeded." << std::endl;

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );
	
	// Create a scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.


	// Create the camera
	rpr_camera camera = nullptr;
	{
		// create the camera
		CHECK( rprContextCreateCamera(context, &camera) );

		// camera position in world space:
		CHECK( rprCameraLookAt(camera, 5, 5, 7,    0, 0, 0,    0, 1, 0) );

		// attach the camera to the scene.
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	
	// Create a simple point light
	rpr_light light=nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Set transform for the light
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(8, 20, 8));
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light power
		CHECK(rprPointLightSetRadiantPower3f(light, 1800, 1800, 1800));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
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

		// Add plane into the scene
		CHECK(rprSceneAttachShape(scene, plane));
	}

	// set a material on the floor
	rpr_material_node diffuseMat=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMat));
		CHECK(rprMaterialNodeSetInputFByKey(diffuseMat, RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.0f));
		CHECK(rprShapeSetMaterial(plane, diffuseMat));
	}

	// create "face A" shape facing the camera
	rpr_shape faceA = nullptr;
	{
		CHECK(rprContextCreateInstance(context,plane,&faceA));

		// Create a transform: 
		RadeonProRender::matrix m = 
			RadeonProRender::translation(RadeonProRender::float3(0, 1.2f, 1.0f)) *
			RadeonProRender::rotation_x(MY_PI/2.0f) *
			RadeonProRender::scale(RadeonProRender::float3(0.0667f, 0.0667f, 0.0667f));

		// Set the transform 
		CHECK(rprShapeSetTransform(faceA, RPR_TRUE, &m.m00));

		CHECK(rprSceneAttachShape(scene, faceA));
	}

	// create "face B" shape back-facing the camera
	rpr_shape faceB = nullptr;
	{
		CHECK(rprContextCreateInstance(context,plane,&faceB));

		// Create a transform: 
		RadeonProRender::matrix m = 
			RadeonProRender::translation(RadeonProRender::float3(1.0f, 1.2f, 0)) *
			RadeonProRender::rotation_z(MY_PI/2.0f) *
			RadeonProRender::scale(RadeonProRender::float3(0.0667f, 0.0667f, 0.0667f));

		// Set the transform 
		CHECK(rprShapeSetTransform(faceB, RPR_TRUE, &m.m00));

		CHECK(rprSceneAttachShape(scene, faceB));
	}

	// material used for front face
	rpr_material_node materialA = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_DIFFUSE,&materialA));
	CHECK( rprMaterialNodeSetInputFByKey(materialA, RPR_MATERIAL_INPUT_COLOR, 1.0f, 0.2f, 0.1f, 0.0f));

	// material used for back face
	rpr_material_node materialB = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_DIFFUSE,&materialB));
	CHECK( rprMaterialNodeSetInputFByKey(materialB, RPR_MATERIAL_INPUT_COLOR, 0.3f, 0.8f, 1.0f, 0.0f));

	// two sided material taking the front and back materials
	rpr_material_node material2sides = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_TWOSIDED,&material2sides));
	CHECK( rprMaterialNodeSetInputNByKey(material2sides, RPR_MATERIAL_INPUT_FRONTFACE, materialA));
	CHECK( rprMaterialNodeSetInputNByKey(material2sides, RPR_MATERIAL_INPUT_BACKFACE, materialB));

	// apply the 2-sided material on both shapes.
	CHECK( rprShapeSetMaterial(faceA, material2sides));
	CHECK( rprShapeSetMaterial(faceB, material2sides));

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800 , 600 }; // resolution in pixels
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; // format: 4 component 32-bit float value each
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) ); // attach 'frame_buffer' to the Color AOV ( this is the main AOV for final rendering )

	// Execute the rendering. 
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 100));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"23_00.png"));
	std::cout << "rendering finished." << std::endl;

	// Release the stuff we created
	CHECK(rprObjectDelete(diffuseMat)); diffuseMat=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	CHECK(rprObjectDelete(materialA)); materialA=nullptr;
	CHECK(rprObjectDelete(materialB)); materialB=nullptr;
	CHECK(rprObjectDelete(material2sides)); material2sides=nullptr;
	CHECK(rprObjectDelete(faceA)); faceA=nullptr;
	CHECK(rprObjectDelete(faceB)); faceB=nullptr;
	CHECK(rprObjectDelete(plane)); plane=nullptr;
	CHECK(rprObjectDelete(light)); light=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}


