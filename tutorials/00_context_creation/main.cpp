/*****************************************************************************\
*
*  Module Name    Context Creation Demo
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
// This demo shows how to create a RPR context.
// The RPR context is the first object that needs to be created before any RPR renderings.
//

int main()
{
	std::cout << "Radeon ProRender SDK simple context creation tutorial.\n";

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

	char deviceName_gpu0[1024]; deviceName_gpu0[0] = 0;
	CHECK(  rprContextGetInfo(context,RPR_CONTEXT_GPU0_NAME, sizeof(deviceName_gpu0), deviceName_gpu0, 0) );

	// Output the name of the GPU
	std::cout << "GPU0 name : " << std::string(deviceName_gpu0) << std::endl;

	// Release the stuff we created
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr; // Always delete the RPR Context in last.

	return 0;
}