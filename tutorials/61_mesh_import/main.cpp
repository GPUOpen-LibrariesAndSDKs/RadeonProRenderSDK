/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright 2011 - 2017 Advanced Micro Devices, Inc. (unpublished)
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Import
#include "../common/common.h"

#include <cassert>
#include <iostream>


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
	CHECK(rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0, NULL, NULL, &context));

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

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK(rprContextCreateScene(context, &scene));
	// Set scene to render for the context
	CHECK(rprContextSetScene(context, scene));

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));

	/////////Mesh Import//////////
	// make sure to execute the demo "60_mesh_export" in order to create the cube_floor.rprs file first.
	CHECK(rprsImport("cube_floor.rprs", context, matsys, &scene, true));

	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer, "61.png"));

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// Try to import your own scene