/*****************************************************************************\
*
*  Module Name    Primvar Demo
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
// Demo of the Primvar. With this feature you can assign additional sets of parameters to rpr_shape.
// Those parameters can be for example: a scalar, 2-float UVs, 3-floats colors.
// They can be uniform to the whole shape, per vertice or per face.
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
	// For this demo we are going to use a simple 1x1 pixel white env light.
	// the image will be created with rprContextCreateImage ( instead of creating it from an image file )
	{
		rpr_light lightEnv = nullptr;
		CHECK( rprContextCreateEnvironmentLight(context, &lightEnv));
		g_gc.GCAdd(lightEnv);
	
		rpr_image_format imageFormat;
		memset(&imageFormat,0,sizeof(imageFormat));
		imageFormat.num_components = 4;
		imageFormat.type = RPR_COMPONENT_TYPE_FLOAT32;

		rpr_image_desc imageDesc;
		memset(&imageDesc,0,sizeof(imageDesc));
		imageDesc.image_depth = 1;
		imageDesc.image_width = 1;
		imageDesc.image_height = 1;
		imageDesc.image_row_pitch = imageDesc.image_width * sizeof(unsigned char)*4;
		imageDesc.image_slice_pitch = imageDesc.image_width * imageDesc.image_height * sizeof(unsigned char)*4;

		// white pixel
		float dataImage[4];
		dataImage[0] = 1.0;
		dataImage[1] = 1.0;
		dataImage[2] = 1.0;
		dataImage[3] = 1.0;

		rpr_image imgEnvLight = nullptr;
		CHECK( rprContextCreateImage(context, imageFormat, &imageDesc, dataImage, &imgEnvLight));
		g_gc.GCAdd(imgEnvLight);

		CHECK( rprEnvironmentLightSetImage(lightEnv, imgEnvLight));
		CHECK( rprEnvironmentLightSetIntensityScale(lightEnv, 1.0f)); 
		CHECK( rprSceneAttachLight(scene, lightEnv));
	}


	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	// create the shape plane.
	// this shape will be a grid so that we can assign primvar set on different vertices/faces.
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

	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800 , 600 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	//
	// Primvar demo of a set defining 1-float per vertex.
	//
	
	float fl1[] = { 
		1.0,  0.2,  0.3,  0.8, 
		0.8,  0.2,  0.2 , 0.7, 
		0.2,  0.2,  0.1,  0.6, 
		0.0,  0.0,  0.0 , 0.0, 
		};

	// We bind this primvar to key=100 
	CHECK( rprShapeSetPrimvar(plane, 100, fl1,  sizeof(fl1)/sizeof(float), 1, RPR_PRIMVAR_INTERPOLATION_VERTEX   ));

	rpr_material_node matPrimvar = NULL;
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_PRIMVAR_LOOKUP, &matPrimvar));
	CHECK( rprMaterialNodeSetInputUByKey(matPrimvar, RPR_MATERIAL_INPUT_VALUE, 100)); // <- specify the primvar key here.
	g_gc.GCAdd(matPrimvar);

	rpr_material_node matDiffuse0 = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &matDiffuse0));
	CHECK( rprMaterialNodeSetInputNByKey(matDiffuse0, RPR_MATERIAL_INPUT_COLOR,  matPrimvar));
	CHECK( rprShapeSetMaterial(plane, matDiffuse0));
	g_gc.GCAdd(matDiffuse0);

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Render the scene
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 100));
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"37_1.png"));


	//
	// Primvar demo of a set defining 3-float (color) per vertex.
	//

	float fl2[] = { 
		1.0,0.0,0.0,     0.0,1.0,0.0,   0.0,0.0,1.0,    0.0,1.0,1.0,
		1.0,0.0,0.0,     0.0,1.0,0.0,   0.0,0.0,1.0,    0.0,1.0,1.0,
		1.0,0.0,0.0,     0.0,1.0,0.0,   0.0,0.0,1.0,    0.0,1.0,1.0,
		1.0,1.0,0.0,     1.0,1.0,0.0,   1.0,1.0,1.0,    1.0,1.0,1.0,
		};
	// assign this set to another key : 101
	CHECK( rprShapeSetPrimvar(plane, 101, fl2,  sizeof(fl2)/sizeof(float), 3, RPR_PRIMVAR_INTERPOLATION_VERTEX   ));

	// as we are currently rendering 'matPrimvar', we need to update the key
	CHECK( rprMaterialNodeSetInputUByKey(matPrimvar, RPR_MATERIAL_INPUT_VALUE, 101));

	// Render the scene
	CHECK( rprFrameBufferClear(frame_buffer) ); // clear the framebuffer before starting a new rendering
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"37_2.png"));

	//
	// Primvar demo of uniform 3-float value
	//

	float fl3[] = { 0.5,0.2,1.0, };	
	CHECK( rprShapeSetPrimvar(plane, 101, fl3,  sizeof(fl3)/sizeof(float), 3, RPR_PRIMVAR_INTERPOLATION_CONSTANT   ));

	// Render the scene
	CHECK( rprFrameBufferClear(frame_buffer) ); // clear the framebuffer before starting a new rendering
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"37_3.png"));

	//
	// Primvar demo of a set defining 3-float (color) per face.
	//

	float fl4[] = { 
		0.0,0.0,1.0,     0.0,1.0,0.0,   1.0,0.0,0.0, 
		1.0,1.0,0.0,     0.0,1.0,1.0,   1.0,0.0,1.0, 
		1.0,1.0,1.0,     0.0,0.0,0.0,   0.0,1.0,1.0,  
		};	
	CHECK( rprShapeSetPrimvar(plane, 101, fl4,  sizeof(fl4)/sizeof(float), 3, RPR_PRIMVAR_INTERPOLATION_UNIFORM   ));

	// Render the scene
	CHECK( rprFrameBufferClear(frame_buffer) ); // clear the framebuffer before starting a new rendering
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK( rprFrameBufferSaveToFile(frame_buffer_resolved,"37_4.png"));


	// Release the stuff we created
	CHECK( rprObjectDelete(camera)); camera=nullptr;
	CHECK( rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK( rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	g_gc.GCClean();
	CHECK( rprObjectDelete(scene)); scene=nullptr;
	CHECK( rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK( rprObjectDelete(context)); context=nullptr;
	return 0;
}




