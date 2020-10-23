/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright 2011 - 2020 Advanced Micro Devices, Inc.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "RprLoadStore.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any fr API calls
	//	frContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK simple rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create OpenCL context using a single GPU 
	rpr_context context = NULL;

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	 status  = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context);

	// Set active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );


	rpr_material_system matsys;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create point light
	rpr_light light = nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 7,5));

		// Set transform for the light
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light radiant power in Watts
		CHECK(rprPointLightSetRadiantPower3f(light, 255, 255, 255));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}
	// Create camera
	rpr_camera camera = nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		// Camera position is (5,5,20)
		// Camera aimed at (0,0,0)
		// Camera up vector is (0,1,0)
		CHECK( rprCameraLookAt(camera, 0, 5, 20, 0, 1, 0, 0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );

	// Create cube mesh
	rpr_shape cube = nullptr;
	{
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube));

		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1.01f, 0));

		// Set the transform 
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
	}
	// Create cube mesh
	rpr_shape cube2 = nullptr;
	{
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube2));

		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube2));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(2, 1.01f, 0));

		// Set the transform 
		CHECK(rprShapeSetTransform(cube2, RPR_TRUE, &m.m00));
	}
	// Create plane mesh
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

	// Create simple diffuse shader
	rpr_material_node diffuse = nullptr;
	{
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse) );

		// Set diffuse color parameter to gray
		CHECK( rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f) );

		// Set shader for cube & plane meshes
		CHECK( rprShapeSetMaterial(plane, diffuse) );
	}
	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer to black color
	CHECK( rprFrameBufferClear(frame_buffer) );

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));


	/////////Volume Tutorial//////////

	//the grid contain value beetween 0 and 1 which are then use to for color, emisive, density
	const uint64_t n = 128; //volume resolution
	std::vector<uint64_t> indices1;
	std::vector<uint32_t> indices2;
	std::vector<float> grid1, grid2;

	const float radiusFadeoutStart = 5.0f;
	const float radiusFadeoutEnd = 40.0f;

	for (int x = 0; x < n; x++)
	{
		for (int y = 0; y < n; y++)
		{
			for (int z = 0; z < n; z++)
			{
				//A simple Cube, there is 1 indices for all voxel, if you skip some it will take a default value of 0
				float v = (float)y / (float)n; //compute the grid value base on y to get a linear vertical gradiant
				//if you move thoses next 2 lines inside the if you will get a cylinder
				indices1.push_back((x)*(n*n) + (y)*(n)+z);
				grid1.push_back(v);


				//Cylinder
				float radius = sqrtf(((float)x - (float)n / 2.0) * ((float)x - (float)n / 2.0) + ((float)z - (float)n / 2.0) * ((float)z - (float)n / 2.0));

				if (radius < radiusFadeoutEnd) // if skiped default value will be 0
				{
					//the RPR_GRID_INDICES_TOPOLOGY_XYZ_U32 is based on 3 indices describing the voxel location, for each 3 index you have 1 value in grid
					indices2.push_back(x);
					indices2.push_back(y);
					indices2.push_back(z);

					if (radius < radiusFadeoutStart)
						grid2.push_back(1.0f);
					else
						grid2.push_back(1.0f - (radius - radiusFadeoutStart) / (radiusFadeoutEnd - radiusFadeoutStart));
				}
			}
		}
	}

	rpr_grid rprgrid1 = 0;
	CHECK(rprContextCreateGrid(context, &rprgrid1,
		n, n, n,
		&indices1[0], indices1.size(), RPR_GRID_INDICES_TOPOLOGY_I_U64,
		&grid1[0], grid1.size() * sizeof(grid1[0]), 0
	));

	rpr_grid rprgrid2 = 0;
	CHECK(rprContextCreateGrid(context, &rprgrid2,
		n, n, n,
		&indices2[0], indices2.size() / 3, RPR_GRID_INDICES_TOPOLOGY_XYZ_U32,
		&grid2[0], grid2.size() * sizeof(grid2[0]), 0
	));


	//Now we have a grid with value we need a way to convert value to color, density and possibly emmision
	//compose of 3 float for 1 value, the repartition is linear, you can insert as many as you want, value will be interpolated
	//look up table save memory as one grid can be bind on multiple channel
	float colorLookUp[] = {
		1.f, 0.0f, 0.0f,//RED
		0.0f, 1.f, 0.0f//GREEN
	};
	//float colorLookUp[] = {
	//	1.f, 0.0f, 0.0f,//RED
	//	0.0f, 1.f, 0.0f,//GREEN
	//	1.f, 0.0f, 0.0f,//RED
	//	0.0f, 1.f, 0.0f,//GREEN
	//	1.f, 0.0f, 0.0f,//RED
	//	0.0f, 1.f, 0.0f,//GREEN
	//	1.f, 0.0f, 0.0f,//RED
	//	0.0f, 1.f, 0.0f//GREEN
	//};

	float densityLookUp[] = {
		10.0f, 10.0f, 10.0f,//DENSITY
	};
	//Density 0 mean 100% transparent
	//float densityLookUp[] = {
	//	0.0f, 0.0f, 0.0f,//DENSITY
	//	1.0f, 1.0f, 1.0f//DENSITY
	//};

	rpr_hetero_volume rprvolume1 = 0;
	CHECK(rprContextCreateHeteroVolume(context, &rprvolume1));


	//Here you can change the rprgrid1 to rprgrid2 to use the 2nd topology
	CHECK(rprHeteroVolumeSetAlbedoGrid(rprvolume1, rprgrid1));
	CHECK(rprHeteroVolumeSetAlbedoLookup(rprvolume1, colorLookUp, 2));//last param is count input, change it if you change the lookup table
	CHECK(rprHeteroVolumeSetDensityGrid(rprvolume1, rprgrid1));
	CHECK(rprHeteroVolumeSetDensityLookup(rprvolume1, densityLookUp, 1));//last param is count input, change it if you change the lookup table

	rpr_hetero_volume rprvolume2 = 0;
	CHECK(rprContextCreateHeteroVolume(context, &rprvolume2));

	CHECK(rprHeteroVolumeSetAlbedoGrid(rprvolume2, rprgrid2));
	CHECK(rprHeteroVolumeSetAlbedoLookup(rprvolume2, colorLookUp, 2));//last param is count input, change it if you change the lookup table
	CHECK(rprHeteroVolumeSetDensityGrid(rprvolume2, rprgrid2));
	CHECK(rprHeteroVolumeSetDensityLookup(rprvolume2, densityLookUp, 1));//last param is count input, change it if you change the lookup table

	//Volume need to be bound to a mesh, if the mesh is transparent the volume will be display
	//We use the same matrix as for the cube
	RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1.01f, 0))
							  * RadeonProRender::scale(RadeonProRender::float3(2.f, 2.f, 2.f)); // The cube is lenght 2 (-1, 1) so we must scale to fill it
	CHECK(rprHeteroVolumeSetTransform(rprvolume1, true, &m.m00));
	m = RadeonProRender::translation(RadeonProRender::float3(2, 1.01f, 0)) * RadeonProRender::scale(RadeonProRender::float3(2.f, 2.f, 2.f));
	CHECK(rprHeteroVolumeSetTransform(rprvolume2, true, &m.m00));

	CHECK(rprSceneAttachHeteroVolume(scene, rprvolume1));
	CHECK(rprSceneAttachHeteroVolume(scene, rprvolume2));
	CHECK(rprShapeSetHeteroVolume(cube, rprvolume1));
	CHECK(rprShapeSetHeteroVolume(cube2, rprvolume2));

	//to avoid dark transparency, we raise recursion
	CHECK(rprContextSetParameterByKey1u(context, RPR_CONTEXT_MAX_RECURSION, 10));
	//Transparency is important for volume, else won't see it
	rpr_material_node materialTransparent = NULL;
	CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_TRANSPARENT, &materialTransparent));
	CHECK(rprMaterialNodeSetInputFByKey(materialTransparent, RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 1.0f));
	CHECK(rprShapeSetMaterial(cube, materialTransparent));
	CHECK(rprShapeSetMaterial(cube2, materialTransparent));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "51.png"));

	// Release the stuff we created
	CHECK(rprObjectDelete(materialTransparent));materialTransparent=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(plane));plane=nullptr;
	CHECK(rprObjectDelete(cube));cube=nullptr;
	CHECK(rprObjectDelete(cube2));cube2=nullptr;
	CHECK(rprObjectDelete(rprgrid1));rprgrid1=nullptr;
	CHECK(rprObjectDelete(rprgrid2));rprgrid2=nullptr;
	CHECK(rprObjectDelete(rprvolume1));rprvolume1=nullptr;
	CHECK(rprObjectDelete(rprvolume2));rprvolume2=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// 1) You can try to change the lookup table (color and density), you also need to change the last parameter of rprHeteroVolumeSet if the number of value in the lookup table changed
// 2) You can try to add a new lookup table for emission rprHeteroVolumeSetEmissionGrid and rprHeteroVolumeSetEmissionLookup
// 3) You can try the to scale value rprHeteroVolumeSetAlbedoScale
// 4) You can try to mix multiple grid, obviously more grid mean higher memory usage