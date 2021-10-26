/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
// Shows how to import an RPR scene as RPRS files ( native RPR file format ) or GLTF ( Khronos Group ).
// It's advised to execute the demo "60_mesh_export" first in order to create the files used in this "61_mesh_import" Demo.
//



#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h" //For Import RPRS files ( native RPR file format )
#include "ProRenderGLTF.h" //For Import from GLTF
#include "../common/common.h"

#include <cassert>
#include <iostream>


int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "-- Radeon ProRender SDK Demo --" << std::endl;
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
	CHECK(rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context));

	// Set active plugin.
	CHECK(rprContextSetActivePlugin(context, plugins[0]));


	rpr_material_system matsys = nullptr;
	CHECK(rprContextCreateMaterialSystem(context, 0, &matsys));
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";


	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));



	///////// RPRS file Import ( native RPR file format ) //////////

	// Create a scene for the RPRS import
	rpr_scene scene_rprs = nullptr;
	CHECK(rprContextCreateScene(context, &scene_rprs));
	CHECK(rprContextSetScene(context, scene_rprs));

	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.rprs file first.
	CHECK(rprsImport("cube_floor.rprs", context, matsys, &scene_rprs, true, nullptr));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering RPRS scene finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "61_rprs.png"));

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// delete the scene
	CHECK(rprObjectDelete(scene_rprs));scene_rprs=nullptr;




	///////// GLTF Import //////////

	// Create a scene for the GLTF import
	rpr_scene scene_gltf = nullptr;
	CHECK(rprContextCreateScene(context, &scene_gltf));
	CHECK(rprContextSetScene(context, scene_gltf));
		
	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.gltf file first.
	CHECK(rprImportFromGLTF("cube_floor.gltf", context, matsys, &scene_gltf, nullptr, 0, nullptr));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering GLTF scene finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "61_gltf.png"));

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprGLTF_DeleteListImportedObjects());

	// delete the scene
	CHECK(rprObjectDelete(scene_gltf));scene_gltf=nullptr;


	


	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

