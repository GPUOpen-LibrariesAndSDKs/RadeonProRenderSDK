/*****************************************************************************\
*
*  Module Name    Parameter Enumeration Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "RadeonProRender.h"

#include "../common/common.h"
#include <iostream>
#include <vector>
#include <string>
#include "../rprTools/RPRStringIDMapper.h"

//
// This tutorials illustrates how to use getters of the RPR API to get current states/values of different objects.
//


int main()
{
	std::cout << "Radeon ProRender SDK parameters enumeration tutorial.\n";

	// Create the RPR context
	rpr_context context = nullptr;
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1);
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0, NULL, NULL, &context) );
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );
	std::cout << "Context successfully created.\n";

	// class defined in rprTools/RPRStringIDMapper.h .
	// this class helps to turn parameters id into strings.
	RPRStringIDMapper stringMapper;


	//
	// List parameters from rpr_context
	//
	{
		std::cout<<std::endl<<"=== CONTEXT PARAMETERS ==="<<std::endl;

		uint64_t param_count = 0;
		CHECK( rprContextGetInfo(context, RPR_CONTEXT_PARAMETER_COUNT, sizeof(uint64_t), &param_count, NULL) );

		std::cout<<param_count<<" parameters :"<<std::endl;

		for (uint64_t i = 0; i < param_count; i++)
		{
			rpr_context_info paramID = (rpr_context_info)0;
			CHECK( rprContextGetParameterInfo(context, int(i), RPR_PARAMETER_NAME, sizeof(paramID), &paramID, NULL) );

			std::string paramName_;
			stringMapper.RPRContextInput_id_to_string(paramID, paramName_);
			
			rpr_parameter_type type;
			CHECK( rprContextGetParameterInfo(context, int(i), RPR_PARAMETER_TYPE, sizeof(type), &type, NULL));

			size_t value_length = 0;
			CHECK( rprContextGetParameterInfo(context, int(i), RPR_PARAMETER_VALUE, 0, NULL, &value_length) );
			char* paramValue = nullptr;
			if (value_length > 0)
			{
				paramValue = new char[value_length];
				CHECK( rprContextGetParameterInfo(context, int(i), RPR_PARAMETER_VALUE, value_length, paramValue, NULL) );
			}

			std::cout<<paramName_<<" = ";

			if ( type == RPR_PARAMETER_TYPE_STRING )
			{
				std::cout<<"\""<<std::string(paramValue)<<"\"";
			}
			else if ( type == RPR_PARAMETER_TYPE_FLOAT )
			{
				float f = *(float*)(paramValue);
				std::cout<<f;
			}
			else if ( type == RPR_PARAMETER_TYPE_FLOAT2 )
			{
				float* f = (float*)(paramValue);
				std::cout<<f[0]<<" , "<<f[1];
			}
			else if ( type == RPR_PARAMETER_TYPE_FLOAT3 )
			{
				float* f = (float*)(paramValue);
				std::cout<<f[0]<<" , "<<f[1]<<" , "<<f[2];
			}
			else if ( type == RPR_PARAMETER_TYPE_FLOAT4 )
			{
				float* f = (float*)(paramValue);
				std::cout<<f[0]<<" , "<<f[1]<<" , "<<f[2]<<" , "<<f[3];
			}
			else if ( type == RPR_PARAMETER_TYPE_UINT )
			{
				rpr_uint v = *(rpr_uint*)(paramValue);
				std::cout<<v;
			}
			else if ( type == RPR_PARAMETER_TYPE_ULONG )
			{
				rpr_ulong v = *(rpr_ulong*)(paramValue);
				std::cout<<v;
			}
			else if ( type == RPR_PARAMETER_TYPE_LONGLONG )
			{
				rpr_longlong v = *(rpr_longlong*)(paramValue);
				std::cout<<v;
			}
			else
			{
				std::cout<<"???";
			}
			
			if ( paramValue ) { delete[] paramValue; paramValue = NULL; }

			std::cout<<std::endl;
		}
	}


	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// Create a MICROFACET material
	rpr_material_node microfacet;
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_MICROFACET, &microfacet) );

	//
	// List parameters from rpr_material
	//
	{
		std::cout<<std::endl<<"=== MATERIAL PARAMETERS ==="<<std::endl;

		// Get the number of parameters for the material
		size_t num_params = 0;
		CHECK( rprMaterialNodeGetInfo(microfacet, RPR_MATERIAL_NODE_INPUT_COUNT, sizeof(size_t), &num_params , NULL));

		std::cout << "Number of MICROFACET material parameters: " << num_params <<"\n";

		// get the material type.
		rpr_material_node_type nodeType = (rpr_material_node_type)0;
		CHECK( rprMaterialNodeGetInfo(microfacet,RPR_MATERIAL_NODE_TYPE, sizeof(nodeType),&nodeType,NULL));
		// in this example, we have  nodeType = RPR_MATERIAL_NODE_MICROFACET

		// For each parameter
		for (size_t i = 0; i < num_params; ++i)
		{
		
			rpr_material_node_input paramNameID = (rpr_material_node_input)0;
			CHECK( rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_NAME, sizeof(paramNameID), &paramNameID, NULL));
			
			std::string paramName_;
			stringMapper.RPRMaterialInput_id_to_string(paramNameID, paramName_);

			std::cout<<paramName_<<" = ";

			rpr_uint nodeInputType = 0;
			CHECK(  rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_TYPE, sizeof(nodeInputType), &nodeInputType, NULL) );

			size_t inputValueSizeB = 0;
			CHECK(  rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_VALUE, 0, NULL, &inputValueSizeB));
			char* materialParameterValue = new char[inputValueSizeB];
			CHECK(  rprMaterialNodeGetInputInfo(microfacet, i, RPR_MATERIAL_NODE_INPUT_VALUE, inputValueSizeB, materialParameterValue, NULL));

			if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_NODE )
			{
				rpr_material_node* mat = (rpr_material_node*)materialParameterValue;
				if ( *mat )
				{
					std::cout<<"material:"<<(void*)(*mat);
				}
				else
				{
					std::cout<<"null-material";
				}
			}
			else if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_FLOAT4 )
			{
				float* f = (float*)(materialParameterValue);
				std::cout<<f[0]<<" , "<<f[1]<<" , "<<f[2]<<" , "<<f[3];
			}
			else
			{
				std::cout<<"???";
			}

			if ( materialParameterValue ) { delete[] materialParameterValue; materialParameterValue = NULL; }

			std::cout<<std::endl;

		}
	}

	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CHECK(rprObjectDelete(microfacet)); microfacet=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}
