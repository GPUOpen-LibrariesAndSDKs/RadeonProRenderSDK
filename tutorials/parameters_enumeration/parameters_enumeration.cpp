/*****************************************************************************\
*
*  Module Name    texturing.cpp
*  Project        Radeon ProRender context texturing tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/

#include "RadeonProRender.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#define CHECK(x) status = x; assert(status == RPR_SUCCESS);

int main()
{
    std::cout << "Radeon ProRender SDK parameters enumeration tutorial.\n";
    // Indicates whether the last operation has suceeded or not
    rpr_int status = RPR_SUCCESS;
	// Create OpenCL context using a single GPU 
	rpr_context context = NULL;

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin("Tahoe64.dll"); 
	assert(tahoePluginID != -1);
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0, NULL, NULL, &context) );

	// Set active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	rpr_material_system matsys;
	status = rprContextCreateMaterialSystem(context, 0, &matsys);
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

    std::cout << "Context successfully created.\n";

    // Create MICROFACET shader
	rpr_material_node microfacet;
	status = rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_MICROFACET, &microfacet);
    assert(status ==  RPR_SUCCESS);

    // Get the number of shader parameters for the shader
    size_t num_params = 0;
    status = rprMaterialNodeGetInfo(microfacet, RPR_MATERIAL_NODE_INPUT_COUNT, sizeof(size_t), &num_params , NULL);
    assert(status ==  RPR_SUCCESS);

    std::cout << "Number of MICROFACET shader parameters: " << num_params <<"\n";

    // For each parameter
    for (size_t i = 0; i < num_params; ++i)
    {
        // Get name
        size_t name_length = 0;
        status = rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_NAME_STRING, 0, NULL, &name_length);
        assert(status ==  RPR_SUCCESS);

        std::vector<rpr_char> name(name_length);
        status = rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_NAME_STRING, name_length, &name[0], NULL);
        assert(status ==  RPR_SUCCESS);

        // Output parameter info
        std::cout << "\n";
        std::cout << "Parameter: " << &name[0] <<"\n";
    }

	CHECK(rprObjectDelete(matsys));
    CHECK(rprObjectDelete(microfacet));
	CHECK(rprObjectDelete(context));
    return 0;
}

// Things to try in this tutorial:
// 1) Try outputting parameters for a layered shader to see a parameter which itself references another shader
