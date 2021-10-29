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
	CHECK(rprContextCreateMaterialSystem(context, 0, &matsys));

	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800,600 };
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );


	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));


	///////// RPRS file Import ( native RPR file format ) //////////

	// Create a scene for the RPRS import
	rpr_scene scene_rprs = nullptr;
	CHECK(rprContextCreateScene(context, &scene_rprs));
	CHECK(rprContextSetScene(context, scene_rprs));

	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.rprs file first.
	CHECK(rprsImport("cube_floor.rprs", context, matsys, &scene_rprs, true, nullptr));

	// Render the scene.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprFrameBufferClear(frame_buffer));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	std::cout << "Rendering RPRS scene finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "61_rprs.png"));

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// delete the scene
	CHECK(rprObjectDelete(scene_rprs));scene_rprs=nullptr;



	
	///////// GLTF Import //////////

	// the scene creation is managed inside rprImportFromGLTF
	rpr_scene scene_gltf = nullptr; 
	
	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.gltf file first.
	CHECK(rprImportFromGLTF("cube_floor.gltf", context, matsys, &scene_gltf, nullptr, 0, nullptr));

	// Render the scene.
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, NUM_ITERATIONS));
	CHECK(rprFrameBufferClear(frame_buffer));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

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

