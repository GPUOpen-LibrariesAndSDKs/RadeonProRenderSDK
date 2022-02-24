/*****************************************************************************\
*
*  Module Name    Light Baking Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2022 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
//
//	This is a demo of how to use the Light Baking feature.
//  This feature is currently only available inside HybridPro.
//
//


#include "RadeonProRender.h"
#include "RadeonProRender_Baikal.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>


// vertex structure with 2 UV layers
// In this demo, the second UV layer is used for light baking.
struct vertexP3N3T2T2
{
	rpr_float pos[3];
	rpr_float norm[3];
	rpr_float tex[2]; // <- used for classic texturing
	rpr_float tex2[2]; // <- used for light baking
};

// create cube with 2 UV layers.
const vertexP3N3T2T2 cube_data__UV2[] = 
{
	{ -1.0f, 1.0f, -1.0f,    0.f, 1.f, 0.f,    0.f, 0.f   ,  0.f, 0.f  },
	{  1.0f, 1.0f, -1.0f,    0.f, 1.f, 0.f,    1.f, 0.f  ,  0.3f, 0.f },
	{  1.0f, 1.0f, 1.0f,     0.f, 1.f, 0.f,    1.f, 1.f  ,   0.3f, 0.3f  },
	{  -1.0f, 1.0f, 1.0f,    0.f, 1.f, 0.f,    0.f, 1.f  ,   0.f, 0.3f },

	{  -1.0f, -1.0f, -1.0f,  0.f, -1.f, 0.f,     0.f, 0.f  , 1.0f*0.3f+  0.f, 0.f  },
	{  1.0f, -1.0f, -1.0f,   0.f, -1.f, 0.f,     1.f, 0.f  , 1.0f*0.3f+    0.3f, 0.f  },
	{  1.0f, -1.0f, 1.0f,    0.f, -1.f, 0.f,     1.f, 1.f  , 1.0f*0.3f+   0.3f, 0.3f },
	{  -1.0f, -1.0f, 1.0f,   0.f, -1.f, 0.f,     0.f, 1.f  , 1.0f*0.3f+   0.f, 0.3f },

	{  -1.0f, -1.0f, 1.0f,   -1.f, 0.f, 0.f,     0.f, 0.f  , 2.0f*0.3f+    0.f, 0.f },
	{  -1.0f, -1.0f, -1.0f,  -1.f, 0.f, 0.f,     1.f, 0.f  , 2.0f*0.3f+    0.3f, 0.f  },
	{  -1.0f, 1.0f, -1.0f,   -1.f, 0.f, 0.f,     1.f, 1.f  , 2.0f*0.3f+    0.3f, 0.3f },
	{  -1.0f, 1.0f, 1.0f,    -1.f, 0.f, 0.f,     0.f, 1.f  , 2.0f*0.3f+   0.f, 0.3f  },

	{  1.0f, -1.0f, 1.0f,     1.f, 0.f, 0.f,    0.f, 0.f   ,  0.f,  1.0f*0.3f+   0.f },
	{  1.0f, -1.0f, -1.0f,    1.f, 0.f, 0.f,    1.f, 0.f   , 0.3f,  1.0f*0.3f+   0.f  },
	{  1.0f, 1.0f, -1.0f,     1.f, 0.f, 0.f,    1.f, 1.f   ,  0.3f, 1.0f*0.3f+  0.3f},
	{  1.0f, 1.0f, 1.0f,      1.f, 0.f, 0.f,    0.f, 1.f   ,  0.f,  1.0f*0.3f+  0.3f },

	{  -1.0f, -1.0f, -1.0f,    0.f, 0.f, -1.f ,    0.f, 0.f  , 1.0f*0.3f+   0.f,  1.0f*0.3f+    0.f },
	{  1.0f, -1.0f, -1.0f,     0.f, 0.f, -1.f ,    1.f, 0.f  , 1.0f*0.3f+   0.3f, 1.0f*0.3f+   0.f },
	{  1.0f, 1.0f, -1.0f,      0.f, 0.f, -1.f,     1.f, 1.f  , 1.0f*0.3f+    0.3f, 1.0f*0.3f+    0.3f  },
	{  -1.0f, 1.0f, -1.0f,     0.f, 0.f, -1.f,     0.f, 1.f  , 1.0f*0.3f+    0.f,  1.0f*0.3f+    0.3f },

	{  -1.0f, -1.0f, 1.0f,   0.f, 0.f, 1.f,     0.f, 0.f  , 2.0f*0.3f+   0.f,  1.0f*0.3f+   0.f  },
	{  1.0f, -1.0f, 1.0f,    0.f, 0.f, 1.f,     1.f, 0.f  , 2.0f*0.3f+   0.3f, 1.0f*0.3f+   0.f   },
	{  1.0f, 1.0f, 1.0f,     0.f, 0.f, 1.f,     1.f, 1.f  , 2.0f*0.3f+   0.3f, 1.0f*0.3f+   0.3f   },
	{  -1.0f, 1.0f, 1.0f,    0.f, 0.f, 1.f,     0.f, 1.f  , 2.0f*0.3f+    0.f, 1.0f*0.3f+   0.3f },
};

const rpr_int cube_data_indices[] = 
{
	3,1,0,
	2,1,3,
	6,4,5,
	7,4,6,
	11,9,8,
	10,9,11,
	14,12,13,
	15,12,14,
	19,17,16,
	18,17,19,
	22,20,21,
	23,20,22
};

const rpr_int cube_data_faces[] = 
{
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};



// build mesh from "P3N3T2T2" structure format.
void BuildMesh_P3N3T2T2(
	rpr_context context_,
	const vertexP3N3T2T2* meshVertices , unsigned int meshVertices_nbOfElement,
	const rpr_int* num_face_vertices  , unsigned int num_face_vertices_nbOfElement,
	const rpr_int* indices,
	rpr_scene scene,
	rpr_shape& mesh
	)
{

	rpr_float const*  uv2[2] = { 
			(rpr_float const*)((char*)&meshVertices[0] + sizeof(rpr_float)*6),
			(rpr_float const*)((char*)&meshVertices[0] + sizeof(rpr_float)*8)
			};

	const size_t num_texcoords[2] = {
		meshVertices_nbOfElement,
		meshVertices_nbOfElement
		};

	const rpr_int texcoord_stride[2] = {
		sizeof(vertexP3N3T2T2),
		sizeof(vertexP3N3T2T2)
		};

	const rpr_int* textcoord_indices[2] = {
		(rpr_int const*)indices,
		(rpr_int const*)indices
		};

	const rpr_int tidx_stride[2] = {
		sizeof(rpr_int),
		sizeof(rpr_int)
		};


	CHECK( rprContextCreateMeshEx2(
		context_,
		(rpr_float const*)&meshVertices[0], meshVertices_nbOfElement , sizeof(vertexP3N3T2T2),
		(rpr_float const*)((char*)&meshVertices[0] + sizeof(rpr_float)*3), meshVertices_nbOfElement, sizeof(vertexP3N3T2T2),
		0,0,0,
		2,
		uv2,  num_texcoords,   texcoord_stride,
		(rpr_int const*)indices, sizeof(rpr_int),
		(rpr_int const*)indices, sizeof(rpr_int),
		textcoord_indices,tidx_stride,
		num_face_vertices, num_face_vertices_nbOfElement,0, &mesh) );
	
	if ( scene ) { CHECK( rprSceneAttachShape(scene, mesh) ); }
	
	return;
}





int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "-- Radeon ProRender SDK Demo --" << std::endl;

	rpr_context context = nullptr;
	
	std::string hybridPluginName = 
	#if defined(WIN32)
	"HybridPro.dll";
	#elif defined(__LINUX__)
	"HybridPro.so";
	#elif defined(__APPLE__)
	""; // no Hybrid plugin released on MacOS
	#endif
	
	// Register the RPR DLL
	rpr_int pluginID = rprRegisterPlugin(hybridPluginName.c_str());
	CHECK_NE(pluginID , -1)
	rpr_int plugins[] = { pluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context));

	// Set active plugin.
	CHECK( rprContextSetActivePlugin(context, plugins[0]));


	// set context parameters for Hybrid Pro
	CHECK( rprContextSetParameterByKey1u(context,(rpr_context_info)RPR_CONTEXT_HIERARCHICAL_LIGHT_SAMPLING_ENABLED,(rpr_uint)1) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_Y_FLIP,(rpr_uint)1) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_MAX_RECURSION,(rpr_uint)5) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_MAX_DEPTH_DIFFUSE,(rpr_uint)5) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_MAX_DEPTH_GLOSSY,(rpr_uint)5) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_MAX_DEPTH_REFRACTION,(rpr_uint)5) );
	CHECK( rprContextSetParameterByKey1f(context,RPR_CONTEXT_DISPLAY_GAMMA,(rpr_float)2.200000f) );
	CHECK( rprContextSetParameterByKey1f(context,RPR_CONTEXT_RAY_CAST_EPISLON,(rpr_float)0.001000f) );
	CHECK( rprContextSetParameterByKey1u(context,(rpr_context_info)RPR_CONTEXT_PT_DENOISER,(rpr_uint)0) );


	// create the scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.

	// create material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 0, 1.2f, 5,    0, 0, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// Get functions dedicated to light baking.
	// Currently they are not directly exposed inside the core RPR API, so we need to use rprContextGetFunctionPtr.
	rprShapeSetLightmapChartIndex_func rprShapeSetLightmapChartIndex___fn = nullptr;
	CHECK( rprContextGetFunctionPtr(context, RPR_SHAPE_SET_LIGHTMAP_CHART_INDEX_FUNC_NAME, (void**)(&rprShapeSetLightmapChartIndex___fn)) );
	rprUvCameraSetChartIndex_func rprUvCameraSetChartIndex___fn = nullptr;
	CHECK( rprContextGetFunctionPtr(context, RPR_UV_CAMERA_SET_CHART_INDEX_FUNC_NAME, (void**)(&rprUvCameraSetChartIndex___fn)) );
	rprContextCreateFramebufferTyped_func rprContextCreateFramebufferTyped___fn = nullptr;
	CHECK( rprContextGetFunctionPtr(context, RPR_CONTEXT_CREATE_FRAMEBUFFER_TYPED_FUNC_NAME, (void**)(&rprContextCreateFramebufferTyped___fn)) );


	// All baked shapes must be assigned to a different index.
	const int floorMeshBakingIndex = 123;



	// Create Floor Shape
	rpr_shape mesh_floor = nullptr;
	rpr_material_node mat_floor = nullptr;
	{
		CHECK(  rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&mat_floor) );  
		CHECK(  rprMaterialNodeSetInputFByKey(mat_floor,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,   1.00f,1.00f,1.00f,   1.0f) );  

		BuildMesh_P3N3T2T2(
			context,
			cube_data__UV2,  24,
			cube_data_faces,  12,
			cube_data_indices,
			scene,
			mesh_floor);

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(0.0f, 0.0f, 0.0f)) * RadeonProRender::scale(RadeonProRender::float3(4.0f, 0.1f, 4.0f));
		CHECK( rprShapeSetTransform(mesh_floor, true, &m.m00) );
		rprShapeSetMaterial(mesh_floor,mat_floor);

		// assign an index to this shape for Light Baking.
		// All baked shapes must be assigned to a different index.
		rprShapeSetLightmapChartIndex___fn(mesh_floor, floorMeshBakingIndex );
	}


	// Create Cube 1
	rpr_shape mesh_cube1 = nullptr;
	rpr_material_node mat_cube1 = nullptr;
	{
		CHECK(  rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&mat_cube1) );  
		CHECK(  rprMaterialNodeSetInputFByKey(mat_cube1,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,   1.00f,0.00f,0.00f,   1.0f) );  

		BuildMesh_P3N3T2T2(
			context,
			cube_data__UV2,  24,
			cube_data_faces,  12,
			cube_data_indices,
			scene,
			mesh_cube1);

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(1.0f, 0.35f, 0.0f)) * RadeonProRender::scale(RadeonProRender::float3(0.2f, 0.2f, 0.2f));
		CHECK(  rprShapeSetTransform(mesh_cube1, true, &m.m00) );
		rprShapeSetMaterial(mesh_cube1,mat_cube1);
	}

	// Create Cube 2
	rpr_shape mesh_cube2 = nullptr;
	rpr_material_node mat_cube2 = nullptr;
	{
		CHECK(  rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&mat_cube2) );  
		CHECK(  rprMaterialNodeSetInputFByKey(mat_cube2,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,   0.00f,1.00f,0.00f,   1.0000f) );  

		BuildMesh_P3N3T2T2(
			context,
			cube_data__UV2,  24,
			cube_data_faces,  12,
			cube_data_indices,
			scene,
			mesh_cube2);

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(0.0f, 0.35f, 1.0f)) * RadeonProRender::scale(RadeonProRender::float3(0.2f, 0.2f, 0.2f));
		CHECK(  rprShapeSetTransform(mesh_cube2, true, &m.m00) );
		rprShapeSetMaterial(mesh_cube2,mat_cube2);

	}

	// Create Cube 3
	rpr_shape mesh_cube3 = nullptr;
	rpr_material_node mat_cube3 = nullptr;
	{
		CHECK(  rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_EMISSIVE,&mat_cube3) );  
		CHECK( rprMaterialNodeSetInputFByKey(mat_cube3,RPR_MATERIAL_INPUT_COLOR,   10.0f,10.0f,0.0f,     1.0f) );  

		BuildMesh_P3N3T2T2(
			context,
			cube_data__UV2,  24,
			cube_data_faces,  12,
			cube_data_indices,
			scene,
			mesh_cube3);

		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(1.0f, 0.3f, 1.0f)) * RadeonProRender::scale(RadeonProRender::float3(0.1f, 0.1f, 0.1f));
		CHECK(  rprShapeSetTransform(mesh_cube3, true, &m.m00 ));
		rprShapeSetMaterial(mesh_cube3,mat_cube3);

	}

	// create the environement light.
	rpr_light env_light = nullptr;
	CHECK(rprContextCreateEnvironmentLight(context, &env_light));
	rpr_image environment_image  = nullptr;
	CHECK( rprContextCreateImageFromFile(context, "../../Resources/Textures/turning_area_4k.hdr", &environment_image));
	CHECK( rprEnvironmentLightSetImage(env_light, environment_image));
	CHECK( rprSceneSetEnvironmentLight(scene, env_light));
	CHECK( rprEnvironmentLightSetIntensityScale(env_light,0.2f) );

 
	// create the framebuffer
	rpr_framebuffer_desc desc = { 800 , 600 }; // resolution in pixels
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; // format: 4 component 32-bit float value each
	rpr_framebuffer frame_buffer = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) ); // attach 'frame_buffer' to the Color AOV ( this is the main AOV for final rendering )



	//
	// Render original scene before baking 
	//
	CHECK( rprFrameBufferClear(frame_buffer) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 1000));
	CHECK( rprContextRender(context) );
	CHECK( rprFrameBufferSaveToFile(frame_buffer, "65_lightbaking_01.png"));


	//
	// Start baking
	//

	// first we need to raise the lightmap flag.
	CHECK(rprContextSetParameterByKey1u(context,  (rpr_context_info)RPR_CONTEXT_ENABLE_LIGHTMAPS,  RPR_TRUE ));

	// we also need a camera dedicated to the baking
	rpr_camera baking_camera = nullptr;
	CHECK( rprContextCreateCamera(context,&baking_camera) );
	CHECK( rprObjectSetName (baking_camera,"baking_camera") );
	CHECK( rprCameraSetMode(baking_camera, (rpr_camera_mode)RPR_CAMERA_MODE_UV)  );
	CHECK( rprSceneSetCamera(scene,baking_camera) );


	rpr_framebuffer_desc lm_desc = {1024 , 1024};
	CHECK(rprUvCameraSetChartIndex___fn( baking_camera ,  floorMeshBakingIndex )); // this baking camera is assigned to the 'mesh_floor'

	rpr_framebuffer baking_framebuffer = nullptr;
	CHECK( rprContextCreateFramebufferTyped___fn(context, 
		RPR_FRAMEBUFFER_TYPE_DOMINANT_DIRECTION_RGB, // <-- choose the lightmap format.
		&lm_desc, &baking_framebuffer));

	CHECK( rprContextSetAOV(context,RPR_AOV_COLOR,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_UV,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_MATERIAL_ID,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_OBJECT_ID,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_EMISSION,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_DIFFUSE_ALBEDO,baking_framebuffer));
	CHECK( rprContextSetAOV(context,RPR_AOV_OPACITY,baking_framebuffer));


	// compute light maps ...
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 2000)); // increase iteration for better light baking quality.
	CHECK( rprContextRender(context) );

	
	//
	// Render the scene WITH baking
	//


	// restore original camera and AOV
	CHECK( rprSceneSetCamera(scene,camera));
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));



	// OPTIONAL : export Light maps
	{
		// CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 2));
		// CHECK( rprContextRender(context) );
		// CHECK( rprFrameBufferSaveToFile(baking_framebuffer,(rpr_char*)"65_lightmap_123.hdr"));
	}


	CHECK( rprFrameBufferClear(frame_buffer) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 1000));
	CHECK( rprContextRender(context) );
	CHECK( rprFrameBufferSaveToFile(frame_buffer, "65_lightbaking_02.png"));


	//
	// In order to illustrate the feature, remove the cubes, and show the baking on the floor
	//

	CHECK( rprSceneDetachShape(scene, mesh_cube1) );
	CHECK( rprSceneDetachShape(scene, mesh_cube2) );
	CHECK( rprSceneDetachShape(scene, mesh_cube3) );
	
	CHECK( rprFrameBufferClear(frame_buffer) );
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 1000));
	CHECK( rprContextRender(context) );
	CHECK( rprFrameBufferSaveToFile(frame_buffer, "65_lightbaking_03.png"));


	// Clean
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(mesh_floor)); mesh_floor=nullptr;
	CHECK(rprObjectDelete(mat_floor)); mat_floor=nullptr;
	CHECK(rprObjectDelete(mesh_cube1)); mesh_cube1=nullptr;
	CHECK(rprObjectDelete(mat_cube1)); mat_cube1=nullptr;
	CHECK(rprObjectDelete(mesh_cube2)); mesh_cube2=nullptr;
	CHECK(rprObjectDelete(mat_cube2)); mat_cube2=nullptr;
	CHECK(rprObjectDelete(mesh_cube3)); mesh_cube3=nullptr;
	CHECK(rprObjectDelete(mat_cube3)); mat_cube3=nullptr;
	CHECK(rprObjectDelete(env_light)); env_light=nullptr;
	CHECK(rprObjectDelete(environment_image)); environment_image=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(baking_camera)); baking_camera=nullptr;
	CHECK(rprObjectDelete(baking_framebuffer)); baking_framebuffer=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;


	return 0;
}

