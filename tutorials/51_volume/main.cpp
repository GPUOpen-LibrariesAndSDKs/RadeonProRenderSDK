/*****************************************************************************\
*
*  Module Name    Volume Demo
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
// This demo demonstrates Volumes with RPR
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

	// Create material system
	rpr_material_system matsys = 0;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys));

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.


	// Create the camera
	rpr_camera camera = nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );
		CHECK( rprCameraLookAt(camera,	2.5f,1.5f,3.5f	, 0.0f, 0.1f, 0.0f,	  0, 1, 0) );
		CHECK( rprSceneSetCamera(scene, camera) );
	}

	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 0.20f, 0.20f,  0.0f,-1.0f,0.0f) );

	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 0.8f) );


	// Create framebuffer 
	rpr_framebuffer_desc desc = { 640 , 480 }; // resolution in pixels
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; // format: 4 component 32-bit float value each
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) ); // attach 'frame_buffer' to the Color AOV ( this is the main AOV for final rendering )


	rpr_mesh_info mesh_properties[16];
	mesh_properties[0] = (rpr_mesh_info)RPR_MESH_VOLUME_FLAG;
	mesh_properties[1] = (rpr_mesh_info)1; // enable the Volume flag for the Mesh
	mesh_properties[2] = (rpr_mesh_info)0;

	// Volume shapes don't need any vertices data: the bounds of volume will only be defined by the grid.
	// Also, make sure to enable the RPR_MESH_VOLUME_FLAG
	rpr_shape cube = 0;
	CHECK( rprContextCreateMeshEx2(context,
		nullptr,0,0,
		nullptr,0,0,
		nullptr,0,0,0,
		nullptr,nullptr,nullptr,nullptr,0,
		nullptr,0,nullptr,nullptr,nullptr,0,
		mesh_properties,
		&cube));

	// bounds of volume will always be a box defined by the rprShapeSetTransform
	RadeonProRender::matrix cubeTransform1 = RadeonProRender::translation(RadeonProRender::float3(0 ,+0.0f ,0))  *  RadeonProRender::rotation_y(0.0f) *   RadeonProRender::scale(RadeonProRender::float3(1.0f, 2.0f, 1.0f));
	CHECK( rprShapeSetTransform(cube,true,&cubeTransform1.m00)); 
	CHECK( rprSceneAttachShape(scene, cube)); 


	// define the grids data used by the Volume material.
	const size_t n = 128;
	std::vector<unsigned int> indicesList;
	std::vector<float> gridVector1;
	std::vector<float> gridVector2;
	const float radiusFadeoutStart = 15.0f;
	const float radiusFadeoutEnd = 40.0f;
	for(unsigned int x=0; x<n; x++)
	{
		for(unsigned int y=0; y<n; y++)
		{
			for(unsigned int z=0; z<n; z++)
			{
				float radius = sqrtf( ((float)x-(float)n/2.0f) * ((float)x-(float)n/2.0f) + ((float)z-(float)n/2.0f) * ((float)z-(float)n/2.0f) );

				if  ( radius < radiusFadeoutEnd )
				{
					indicesList.push_back(x);
					indicesList.push_back(y);
					indicesList.push_back(z);
					
					// "gridVector1" is going to be a cylinder
					if ( radius <= radiusFadeoutStart )
					{
						gridVector1.push_back(1.0f);
					}
					else
					{
						gridVector1.push_back(1.0f - (radius - radiusFadeoutStart) / (radiusFadeoutEnd-radiusFadeoutStart));
					}
					
					// "gridVector2" will be a 0->1 ramp along Y-axis
					gridVector2.push_back(  (float)y / (float)n );
				}
			}
		}
	}

	// this first grid defines a cylinder
	rpr_grid rprgrid1 = 0;
	CHECK( rprContextCreateGrid(context, &rprgrid1,
		n,n,n,
		&indicesList[0], indicesList.size()/3, RPR_GRID_INDICES_TOPOLOGY_XYZ_U32,
		&gridVector1[0], gridVector1.size() * sizeof(gridVector1[0]), 0
		));

	// GRID_SAMPLER could be compared to a 3d-texture sampler. 
	// input is a 3d grid,  output is the sampled value from grid
	rpr_material_node gridSampler1 = NULL;
	CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_GRID_SAMPLER, &gridSampler1));
	CHECK( rprMaterialNodeSetInputGridDataByKey(gridSampler1, RPR_MATERIAL_INPUT_DATA, rprgrid1));

	// This second grid is a gradient along the Y axis.
	rpr_grid rprgrid2 = 0;
	CHECK( rprContextCreateGrid(context, &rprgrid2,
		n,n,n,
		&indicesList[0], indicesList.size()/3, RPR_GRID_INDICES_TOPOLOGY_XYZ_U32,
		&gridVector2[0], gridVector2.size() * sizeof(gridVector2[0]), 0
		));

	// create grid sample for grid2
	rpr_material_node gridSampler2 = NULL;
	CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_GRID_SAMPLER, &gridSampler2));
	CHECK( rprMaterialNodeSetInputGridDataByKey(gridSampler2, RPR_MATERIAL_INPUT_DATA, rprgrid2));

	// create a gradient color texture, here 3 pixels : Red, Green, Blue.
	// will be used as a lookup output 
	float rampData2[] = {
		1.f,0.f,0.f,
		0.f,1.f,0.f,
		0.f,0.f,1.f};
	rpr_image rampimg2 = 0;
	rpr_image_desc rampDesc2;
	rampDesc2.image_width = sizeof(rampData2) / (3*sizeof(float));
	rampDesc2.image_height = 1;
	rampDesc2.image_depth = 0;
	rampDesc2.image_row_pitch = rampDesc2.image_width * sizeof(rpr_float) * 3;
	rampDesc2.image_slice_pitch = 0;
	CHECK(  rprContextCreateImage(context, {3, RPR_COMPONENT_TYPE_FLOAT32}, &rampDesc2, rampData2, &rampimg2));

	// this texture will be used for the color of the volume material.
	// UV input is the 0->1 gradient created by the scalar grid "rprgrid2".
	// Output is the red,green,blue texture.
	// This demonstrates how we can create a lookup table from scalar grid to vector values.
	rpr_material_node rampSampler2 = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &rampSampler2));
	CHECK( rprMaterialNodeSetInputImageDataByKey(rampSampler2, RPR_MATERIAL_INPUT_DATA, rampimg2));
	CHECK( rprMaterialNodeSetInputNByKey(rampSampler2, RPR_MATERIAL_INPUT_UV, gridSampler2));

	// for ramp texture, it's better to clamp it to edges.
	CHECK( rprMaterialNodeSetInputUByKey(rampSampler2, RPR_MATERIAL_INPUT_WRAP_U, RPR_IMAGE_WRAP_TYPE_CLAMP_TO_EDGE));
	CHECK( rprMaterialNodeSetInputUByKey(rampSampler2, RPR_MATERIAL_INPUT_WRAP_V, RPR_IMAGE_WRAP_TYPE_CLAMP_TO_EDGE));

	// create the Volume material
	rpr_material_node materialVolume = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_VOLUME, &materialVolume));

	// density is defined by the "cylinder" grid
	CHECK( rprMaterialNodeSetInputNByKey(materialVolume, RPR_MATERIAL_INPUT_DENSITYGRID, gridSampler1));

	// apply the volume material to the shape.
	// Note that here we use   rprShapeSetVolumeMaterial  instead of the classic  rprShapeSetMaterial  call.
	CHECK( rprShapeSetVolumeMaterial(cube, materialVolume));

	// RPR_MATERIAL_INPUT_DENSITY is just a multiplier for DENSITYGRID
	CHECK( rprMaterialNodeSetInputFByKey(materialVolume, RPR_MATERIAL_INPUT_DENSITY, 10.0f, 0.0f, 0.0f, 0.0f));

	// define the color of the volume
	CHECK( rprMaterialNodeSetInputNByKey(materialVolume, RPR_MATERIAL_INPUT_COLOR, rampSampler2));

	// more iterations will increase the light penetration inside the volume.
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_MAX_RECURSION, (rpr_uint)5) ); // 5

	// when using volumes, we usually need high number of iterations.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 3000));

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Start the rendering. 
	CHECK( rprContextRender(context) );

	// resolve and save the rendering to an image file.
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"51_00.png"));


	// Release the stuff we created
	CHECK(rprObjectDelete(cube)); cube=nullptr;
	CHECK(rprObjectDelete(rprgrid1)); rprgrid1=nullptr;
	CHECK(rprObjectDelete(gridSampler1)); gridSampler1=nullptr;
	CHECK(rprObjectDelete(rprgrid2)); rprgrid2=nullptr;
	CHECK(rprObjectDelete(gridSampler2)); gridSampler2=nullptr;
	CHECK(rprObjectDelete(materialVolume)); materialVolume=nullptr;
	CHECK(rprObjectDelete(rampimg2)); rampimg2=nullptr;
	CHECK(rprObjectDelete(rampSampler2)); rampSampler2=nullptr;
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


