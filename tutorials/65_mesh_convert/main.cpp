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


int main(int argc, char *argv[])
{
	if( argc < 3 )
		return 0;
	const std::string src = argv[1];
	const std::string dst = argv[2];

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

	std::cout << "Converting from  " << src << std::endl;
	std::cout << "             to  " << dst << std::endl;
	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.rprs file first.
	CHECK(rprsImport(src.c_str(), context, matsys, &scene_rprs, true, nullptr));

	///////// GLTF Export //////////
	CHECK(rprExportToGLTF(dst.c_str(), context, nullptr, &scene_rprs, 1, 0, nullptr));

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// delete the scene
	CHECK(rprObjectDelete(scene_rprs));scene_rprs=nullptr;

	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

