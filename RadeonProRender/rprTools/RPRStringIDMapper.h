/*****************************************************************************\
*
*  Module Name    RPRStringIDMapper.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#pragma once

#include <string>
#include <unordered_map>

#include <RadeonProRender.h>

//for all applications that need to convert RPR API define integer, into string, it's advised to use those functions from rprTools, so that all strings are unified across projects.

class RPRStringIDMapper
{
public:

	RPRStringIDMapper();

	void RPRMaterialInput_id_to_string(rpr_material_node_input id, std::string& strOut); // strOut = "" if fail
	rpr_material_node_input RPRMaterialInput_string_to_id(const std::string& strIn); // returns -1 if fail

	void RPRContextInput_id_to_string(rpr_context_info id, std::string& strOut); // strOut = "" if fail
	rpr_context_info RPRContextInput_string_to_id(const std::string& strIn); // returns -1 if fail

	void RPRMaterialType_id_to_string(rpr_material_node_type id, std::string& strOut); // strOut = "" if fail
	rpr_material_node_type RPRMaterialType_string_to_id(const std::string& strIn); // returns -1 if fail

private:
	std::unordered_map<std::string,rpr_material_node_input> m_RPRMaterialInput_string_to_id;
	std::unordered_map<rpr_material_node_input,std::string> m_RPRMaterialInput_id_to_string;

	std::unordered_map<std::string,rpr_context_info> m_RPRContextInput_string_to_id;
	std::unordered_map<rpr_context_info,std::string> m_RPRContextInput_id_to_string;

	std::unordered_map<std::string,rpr_material_node_type> m_RPRMaterialType_string_to_id;
	std::unordered_map<rpr_material_node_type,std::string> m_RPRMaterialType_id_to_string;
};