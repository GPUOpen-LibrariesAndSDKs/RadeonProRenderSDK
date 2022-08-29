/*****************************************************************************\
*
*  Module Name    Per Face Material Demo
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
// Demo of the rprShapeSetMaterialFaces API. This allows to set materials for specific faces of the shape.
//

// garbage collector of this demo
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

	// Create context
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set the active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "RPR Context creation succeeded." << std::endl;

	// Create the scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); 
	CHECK( rprContextSetScene(context, scene) );

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 0, 10, 12,    0, -2, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	
	// Create an environment light
	CHECK( CreateNatureEnvLight(context, scene, g_gc, 1.0f) );

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );



	// create the shape plane 
	rpr_shape plane = NULL;
	{

		const int gridSize = 4; // create a 3x3 grid (4*4 vertices)
		vertex plane_data[gridSize*gridSize];

		// grid dimensions
		const float minGrid = -5.0f;
		const float maxGrid = +5.0f;

		// create grid
		for(int y=0; y<gridSize; y++)
		{
			for(int x=0; x<gridSize; x++)
			{
				plane_data[y*gridSize+x].pos[0] = minGrid + (maxGrid-minGrid) / ((float)gridSize-1.0f) * ((float)x);
				plane_data[y*gridSize+x].pos[1] = 0.0f;
				plane_data[y*gridSize+x].pos[2] = minGrid + (maxGrid-minGrid) / ((float)gridSize-1.0f) * ((float)y);

				plane_data[y*gridSize+x].norm[0] = 0.0f;
				plane_data[y*gridSize+x].norm[1] = 1.0f;
				plane_data[y*gridSize+x].norm[2] = 0.0f;

				plane_data[y*gridSize+x].tex[0] = (plane_data[y*gridSize+x].pos[0] - minGrid) / (maxGrid - minGrid);
				plane_data[y*gridSize+x].tex[1] = 1.0f - (plane_data[y*gridSize+x].pos[2] - minGrid) / (maxGrid - minGrid);
			}
		}

		// create indices of the grid
		rpr_int indices[(gridSize-1)*(gridSize-1)*4];
		for(int y=0; y<gridSize-1; y++)
		{
			for(int x=0; x<gridSize-1; x++)
			{
				indices[  (x+y*(gridSize-1))*4 + 0  ] = y*gridSize+x  ;
				indices[  (x+y*(gridSize-1))*4 + 1  ] = y*gridSize+(x+1)  ;
				indices[  (x+y*(gridSize-1))*4 + 3  ] = (y+1)*gridSize+x  ;
				indices[  (x+y*(gridSize-1))*4 + 2  ] = (y+1)*gridSize+(x+1)  ;
			}
		}

		// create face list
		rpr_int num_face_vertices[(gridSize-1)*(gridSize-1)];
		for(int y=0; y<gridSize-1; y++)
		{
			for(int x=0; x<gridSize-1; x++)
			{
				num_face_vertices[  (x+y*(gridSize-1))  ] = 4; // use quad face: 4 vertices per face.
			}
		}

		CHECK( rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], gridSize*gridSize, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*3), gridSize*gridSize, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float)*6), gridSize*gridSize, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, (gridSize-1)*(gridSize-1), &plane) );
		g_gc.GCAdd(plane);

		// attach the plane to the scene.
		CHECK( rprSceneAttachShape(scene, plane));

	}


	// create the base material for the plane shape
	{
		rpr_material_node diffuseMaterial = nullptr;
		rpr_image image2 = nullptr;
		rpr_material_node materialImage2 = nullptr;

		const std::string pathImageFileA = "../../Resources/Textures/flower.jpg";
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFileA.c_str(), &image2);
		g_gc.GCAdd(image2);
		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFileA << " not found.\n";
			return status;
		}
		CHECK(status);

		// This texture is using the classic 2.2 gamma compression.
		CHECK( rprImageSetGamma(image2, 2.2f) );

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialImage2));
		g_gc.GCAdd(materialImage2);
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialImage2, RPR_MATERIAL_INPUT_DATA, image2)); // Set image data

		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMaterial));
		g_gc.GCAdd(diffuseMaterial);
		CHECK(rprMaterialNodeSetInputNByKey(diffuseMaterial, RPR_MATERIAL_INPUT_COLOR, materialImage2)); // set image sampler as the color input of diffuse material


		// assign the material on the shape
		CHECK(rprShapeSetMaterial(plane, diffuseMaterial));

	}

	// create a simple Red diffuse material
	rpr_material_node diffuseRed = NULL;
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE,&diffuseRed) );
	g_gc.GCAdd(diffuseRed);
	CHECK( rprMaterialNodeSetInputFByKey(diffuseRed, RPR_MATERIAL_INPUT_COLOR, 1, 0.1, 0.05f, 1) );

	// create a simple Blue diffuse material
	rpr_material_node diffuseBlue = NULL;
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE,&diffuseBlue) );
	g_gc.GCAdd(diffuseBlue);
	CHECK( rprMaterialNodeSetInputFByKey(diffuseBlue, RPR_MATERIAL_INPUT_COLOR, 0.05f, 0.1f, 1, 1) );

	// assign the Red material to faces '0' and '2'
	rpr_int indicesList1[] = { 0, 2 };
	CHECK( rprShapeSetMaterialFaces(plane, diffuseRed, indicesList1, sizeof(indicesList1) / sizeof(indicesList1[0]) ) );

	// assign the Blue material to faces '5', '6' and '7'
	rpr_int indicesList2[] = { 5, 6, 7 };
	CHECK( rprShapeSetMaterialFaces(plane, diffuseBlue, indicesList2, sizeof(indicesList2) / sizeof(indicesList2[0]) ) );

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
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"34.png"));


	// Release the stuff we created
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




