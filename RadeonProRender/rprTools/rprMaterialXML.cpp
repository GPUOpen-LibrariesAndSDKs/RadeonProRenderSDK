/*****************************************************************************\
*
*  Module Name    rprMaterialXML.cpp
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "rprMaterialXML.h"
#include "tinyxml2.h"
#include "RPRStringIDMapper.h"
#include <vector>
#include <fstream>
#include <unordered_set>
#include "Math/float3.h"
#include "Math/float4.h"

#define MACRO__IF_TYPE_THEN_SET(a)  else if ( nodeElement_type == #a ) { newNode.materialType = RPR_MATERIAL_NODE_##a; }

#define MACRO_CHECK_RPR_STATUS  if ( status != RPR_SUCCESS )  { throw (rpr_int)status; }




// example : convert  "0.662745, 0.109804, 0.0196078, 0"  to   4 floats
RadeonProRender::float4 rprx4FloatFromXMLString(const std::string& f4, bool& success)
{
	success = false;

	std::string f4_ = f4;

	int nbComma = 0;
	for(int i=0; i<f4_.length(); i++)
	{
		if ( f4_[i] == ',' )
		{
			f4_[i] = ' ';
			nbComma++;
		}
	}

	if ( nbComma != 3 )
	{
		success = false;
		return RadeonProRender::float4(0.0f,0.0f,0.0f,0.0f);
	}

	std::string::size_type sz;     // alias of size_t
	float x = std::stof(f4_,&sz);
	f4_ = f4_.substr(sz);
	float y = std::stof(f4_,&sz);
	f4_ = f4_.substr(sz);
	float z = std::stof(f4_,&sz);
	f4_ = f4_.substr(sz);
	float w = std::stof(f4_,&sz);

	success = true;

	return RadeonProRender::float4(x,y,z,w);
}

rpr_uint rprxUintFromXMLString(const std::string& ui, bool& success)
{
	success = true;

	rpr_uint ret = std::stoul(ui);

	return ret;

}

rpr_int rprtools_MaterialXMLImport(
	rpr_context context, 
	rpr_material_system matSystem, 
	const char* imageBaseFolderPath,
	const char* xmlFilePath, 
	int& masterMaterialIndex,
	std::vector<RPR_TOOL_NODE_MATERIAL_IMPORTED>& matNodeList, 
	int& masterMaterialIndex_displacement,
	std::vector<rpr_material_node>& extraArithmeticNodes
	)
{
	try
	{
		RPRStringIDMapper strIdMapper;

		rpr_int status = RPR_SUCCESS;

		// reset outputs
		masterMaterialIndex = -1;
		matNodeList.clear();
		extraArithmeticNodes.clear();
		masterMaterialIndex_displacement = -1;

		std::string displacementNodeUsed;

		tinyxml2::XMLDocument doc;
		doc.LoadFile( xmlFilePath ); 
		tinyxml2::XMLNode* rootnode = doc.FirstChild();
		tinyxml2::XMLNode* SiblingNode = rootnode->NextSibling();
		tinyxml2::XMLElement* material_element = SiblingNode->ToElement();

		std::string material_element_name = material_element->Name();
		if ( material_element_name != "material" )
			throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

		std::string  material_element_attib_name		 = material_element->Attribute("name");
		std::string  material_element_attib_vexp		 = material_element->Attribute("version_exporter");
		std::string  material_element_attib_vrpr		 = material_element->Attribute("version_rpr");
		std::string  material_element_attib_clos		 = material_element->Attribute("closure_node"); // closure_node is the name of the node containing the final output of the material
		
		std::string  material_element_attib_displacement;
		if ( material_element->FindAttribute("displacement_node") )
		{
			material_element_attib_displacement = material_element->Attribute("displacement_node");
		}

		//if not version 1.0... maybe something needs to be done in this parser source code ?
		if ( material_element_attib_vexp != "10" )
			throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

		tinyxml2::XMLNode* ChildNode = SiblingNode ->FirstChild();
		tinyxml2::XMLElement* ChildElmt = ChildNode->ToElement();
		const char* strTagName = ChildElmt->Name(); // "description"
		
		if ( std::string(strTagName) != "description" )
			throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
		
		tinyxml2::XMLNode* node2 = ChildNode->NextSibling();

		bool containsDisplacement = false;

		for(unsigned int iNode_=0; ;iNode_++)
		{

			tinyxml2::XMLElement* node2_element = node2->ToElement();
			std::string node2_element_name = node2_element->Name();
			
			if ( node2_element_name != "node" )
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

			tinyxml2::XMLElement* nodeElement = node2_element->ToElement();
			std::string  nodeElement_name = nodeElement->Attribute("name");
			std::string  nodeElement_type = nodeElement->Attribute("type");

			if ( nodeElement_name == material_element_attib_clos )
			{
				// closure node shouldn't be already set
				if ( masterMaterialIndex != -1 )
					throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

				masterMaterialIndex = iNode_;
			}

			if ( material_element_attib_displacement != "" && nodeElement_name == material_element_attib_displacement )
			{
				// closure node shouldn't be already set
				if ( masterMaterialIndex_displacement != -1 )
					throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

				masterMaterialIndex_displacement = iNode_;
			}

			RPR_TOOL_NODE_MATERIAL_IMPORTED newNode;
			newNode.nodeName = nodeElement_name;

			if ( nodeElement_type == "UBER" ) { newNode.materialType = RPR_MATERIAL_NODE_UBERV2; } // retro compatibility with old naming "UBER"
			else if ( nodeElement_type == "INPUT_TEXTURE" ) 
			{ 
				newNode.materialType = (rpr_material_node_type)0; 
			}
			else
			{
				newNode.materialType = strIdMapper.RPRMaterialType_string_to_id(nodeElement_type);
				
				if ( newNode.materialType == -1 )
					throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

			}

			
			if ( nodeElement_type == "INPUT_TEXTURE" ) 
			{ 

			}
			else
			{
				newNode.matNode = nullptr;	
				status = rprMaterialSystemCreateNode(matSystem,newNode.materialType,&newNode.matNode);  MACRO_CHECK_RPR_STATUS;
				status = rprObjectSetName(newNode.matNode, newNode.nodeName.c_str());  MACRO_CHECK_RPR_STATUS;
			}



			tinyxml2::XMLNode* paramFirstChild = node2->FirstChild();

			for(unsigned int iNodeParam=0; ;iNodeParam++)
			{
				tinyxml2::XMLElement* paramFirstChild_element = paramFirstChild->ToElement();
				if ( paramFirstChild_element == nullptr )
				{
					// if we reach this case, we are probably inside an XML comment
				}
				else
				{
					std::string paramFirstChild_element_name = paramFirstChild_element->Name();

					if ( paramFirstChild_element_name != "param")
						throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			
					std::string  name = paramFirstChild_element->Attribute("name");
					std::string  type = paramFirstChild_element->Attribute("type");
					std::string  value = paramFirstChild_element->Attribute("value");

					if ( name == "displacement" )
					{
						containsDisplacement = true;
					}

					if ( nodeElement_type == "INPUT_TEXTURE" ) 
					{
						if ( name == "path" && type == "file_path" )
						{
							std::string fullPathName = std::string(imageBaseFolderPath) + value;
							status = rprContextCreateImageFromFile(context,fullPathName.c_str(),&newNode.image); MACRO_CHECK_RPR_STATUS;
							status = rprObjectSetName(newNode.image, value.c_str());  MACRO_CHECK_RPR_STATUS;
							newNode.imagePath = value;
						}
						else if ( name == "gamma" && type == "float" )
						{
							newNode.imageGamma = std::stof(value);
						}
						else if ( name == "tiling_u" && type == "float" )
						{
							newNode.tilingX = std::stof(value);
						}
						else if ( name == "tiling_v" && type == "float" )
						{
							newNode.tilingY = std::stof(value);
						}
						else
						{
							throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
						}
					}
					else
					{
						if ( nodeElement_type == "UBER" && name == "displacement" ) // manage old verison of XML : when displacement was an input of UBER. ( is recent verison of RPR, we don't have that anymore )
						{
							if ( type == "connection" )
							{
								displacementNodeUsed = value;
							}
							else
							{
								throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
							}
						}
						else if ( type == "uint" )
						{
							unsigned long val = std::stoul(value);
							status = rprMaterialNodeSetInputUByKey(newNode.matNode, strIdMapper.RPRMaterialInput_string_to_id( name ),val);  
							MACRO_CHECK_RPR_STATUS;
						}
						else if ( type == "connection" )
						{
							newNode.connecNode_.push_back( std::pair< std::string , std::pair<std::string,rpr_material_node_input> >(value,  std::pair<std::string,rpr_material_node_input>(name, (rpr_material_node_input)0) ) );
						}
						else if ( type == "float4" )
						{
							bool success = false;
							RadeonProRender::float4 f4 = rprx4FloatFromXMLString(value,success);
							if ( !success )
								throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
							status = rprMaterialNodeSetInputFByKey(newNode.matNode, strIdMapper.RPRMaterialInput_string_to_id(name),f4.x , f4.y , f4.z, f4.w);  
							MACRO_CHECK_RPR_STATUS;
						}
						else
						{
							throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
						}

					}
				}

				paramFirstChild = paramFirstChild->NextSibling();
				if ( paramFirstChild == nullptr )
					break;
			}

			matNodeList.push_back(newNode);

			node2 = node2->NextSibling();
			if ( node2 == nullptr )
				break;
		}

		//scale UV when needed
		for(unsigned int iNode_=0; iNode_<matNodeList.size();iNode_++) // for each nodes
		{
			if (
					matNodeList[iNode_].matNode
				&& matNodeList[iNode_].materialType == RPR_MATERIAL_NODE_IMAGE_TEXTURE  // if it's a RPR Node for Image
				)
			{
				for (auto const& x : matNodeList[iNode_].connecNode_) // for each node connected to this node
				{
					//search node
					int nodeToConnect = -1;
					for(unsigned int jNode_=0; jNode_<matNodeList.size();jNode_++)
					{
						if ( matNodeList[jNode_].nodeName == x.first )
						{
							nodeToConnect = jNode_;
							break;
						}
					}
					if (nodeToConnect == -1)
						throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

					if (
							matNodeList[nodeToConnect].image // if the node connected is an rpr_image
						&& (
							matNodeList[nodeToConnect].tilingX !=  1.0f 
						|| matNodeList[nodeToConnect].tilingY !=  1.0f // if this image has scaling
							)
						)
					{
						float scaleUVx = matNodeList[nodeToConnect].tilingX;
						float scaleUVy = matNodeList[nodeToConnect].tilingY;

						char autoName[128];
						
						rpr_material_node uv_node = NULL;
						status = rprMaterialSystemCreateNode(matSystem, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node);MACRO_CHECK_RPR_STATUS;
						sprintf(autoName,"node_autoname_0x%p",(void*)uv_node);
						status = rprObjectSetName(uv_node, autoName);  MACRO_CHECK_RPR_STATUS;
						extraArithmeticNodes.push_back(uv_node);
						status = rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV);MACRO_CHECK_RPR_STATUS;

						rpr_material_node uvScaled = NULL;
						status = rprMaterialSystemCreateNode(matSystem, RPR_MATERIAL_NODE_ARITHMETIC, &uvScaled);MACRO_CHECK_RPR_STATUS;
						sprintf(autoName,"node_autoname_0x%p",(void*)uvScaled);
						status = rprObjectSetName(uvScaled, autoName);  MACRO_CHECK_RPR_STATUS;
						extraArithmeticNodes.push_back(uvScaled);
						status = rprMaterialNodeSetInputUByKey(uvScaled, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL);MACRO_CHECK_RPR_STATUS;
						status = rprMaterialNodeSetInputNByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR0, uv_node);MACRO_CHECK_RPR_STATUS;
						status = rprMaterialNodeSetInputFByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR1, scaleUVx, scaleUVy, 0.0f, 0);MACRO_CHECK_RPR_STATUS;

						status = rprMaterialNodeSetInputNByKey(matNodeList[iNode_].matNode, RPR_MATERIAL_INPUT_UV, uvScaled ); MACRO_CHECK_RPR_STATUS;
					}
				}
			}
		}


		//now, connect nodes
		for(unsigned int iNode_=0; iNode_<matNodeList.size();iNode_++)
		{

			if ( displacementNodeUsed == matNodeList[iNode_].nodeName )
			{
				masterMaterialIndex_displacement = iNode_;
			}

			//also apply image parameters
			if ( matNodeList[iNode_].image && matNodeList[iNode_].imageGamma != 1.0f )
			{
				status = rprImageSetGamma(matNodeList[iNode_].image,  matNodeList[iNode_].imageGamma ); MACRO_CHECK_RPR_STATUS;
			}


			for (auto const& x : matNodeList[iNode_].connecNode_)
			{

				//search node
				int nodeToConnect = -1;
				for(unsigned int jNode_=0; jNode_<matNodeList.size();jNode_++)
				{
					if ( matNodeList[jNode_].nodeName == x.first )
					{
						nodeToConnect = jNode_;
						break;
					}
				}

				if ( nodeToConnect == -1 )
					throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;

				if ( matNodeList[iNode_].matNode )
				{
					std::string paramName = x.second.first;

					if ( matNodeList[nodeToConnect].matNode )
					{
						status = rprMaterialNodeSetInputNByKey(matNodeList[iNode_].matNode,  strIdMapper.RPRMaterialInput_string_to_id(paramName), matNodeList[nodeToConnect].matNode );  
						MACRO_CHECK_RPR_STATUS;
					}
					else if ( matNodeList[nodeToConnect].image )
					{
						status = rprMaterialNodeSetInputImageDataByKey(matNodeList[iNode_].matNode,  strIdMapper.RPRMaterialInput_string_to_id(paramName), matNodeList[nodeToConnect].image );  
						MACRO_CHECK_RPR_STATUS;
					}
					else if ( matNodeList[nodeToConnect].light )
					{
						status = rprMaterialNodeSetInputLightDataByKey(matNodeList[iNode_].matNode,  strIdMapper.RPRMaterialInput_string_to_id(paramName), matNodeList[nodeToConnect].light );  
						MACRO_CHECK_RPR_STATUS;
					}
					else 
					{
						throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
					}
				}
				else
				{
					throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
				}
					
			}
			
		}
		
		
		// check we have a closure node.
		if ( masterMaterialIndex == -1 )
			throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;


	}
	catch(std::exception& e)
	{
		return RPR_ERROR_INTERNAL_ERROR;
	}
	catch(rpr_int e)
	{
		return e;
	}

	return RPR_SUCCESS;
}









struct RPRTOOLS_NODE_EXPORT_DEFINE
{
	RPRTOOLS_NODE_EXPORT_DEFINE()
	{
		rprMat = nullptr;
		rprImage = nullptr;
	}

	std::string name; // "name" property for the XML file
	std::string type;

	rpr_material_node rprMat;
	rpr_image rprImage;

	struct RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE
	{
		std::string name;
		std::string type;
		std::string value;
	};

	std::vector<RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE> paramList;

	
};

// make sure the node name is unique
void GiveUniqueNodeName(
	std::unordered_set<std::string>& xmlNodeNames, 
	const std::string& nameBase_,
	std::string& nameOut)
{
	std::string nameBase = nameBase_;
	if ( nameBase == "" )
	{
		nameBase = "node";
	}
	std::string nameCandidate = nameBase;
	for(int i=0; ; i++)
	{
		if ( xmlNodeNames.find(nameCandidate) == xmlNodeNames.end() )
		{
			break;
		}
		nameCandidate = nameBase + std::to_string(i);
	}
	xmlNodeNames.insert(nameCandidate);
	nameOut = nameCandidate;
	
	return;
}

void XMLExporter_fillImageList(
	rpr_image imgNodeIn, 
	std::unordered_map<void*,RPRTOOLS_NODE_EXPORT_DEFINE>& nodesSet, 
	RPRStringIDMapper& strIdMapper,
	std::unordered_set<std::string>& xmlNodeNames
	)
{
	// if image already in export list
	if ( nodesSet.find(imgNodeIn) != nodesSet.end() )
	{
		return;
	}

	rpr_int status = RPR_SUCCESS;

	RPRTOOLS_NODE_EXPORT_DEFINE newNode;
	newNode.rprImage = imgNodeIn;
	newNode.type = "INPUT_TEXTURE";

	std::string rprObjectName;

	{
		size_t frobjectName_size = 0;
		status = rprImageGetInfo(imgNodeIn, RPR_IMAGE_NAME, NULL, NULL, &frobjectName_size); MACRO_CHECK_RPR_STATUS;
		if ( frobjectName_size <= 0 ) { throw (rpr_int)RPR_ERROR_INTERNAL_ERROR; } // because of 0 terminated character, size must be >= 1
		char* frobjectName_data = new char[frobjectName_size];
		status = rprImageGetInfo(imgNodeIn, RPR_IMAGE_NAME, frobjectName_size, frobjectName_data, NULL); MACRO_CHECK_RPR_STATUS;
		if ( frobjectName_data[frobjectName_size-1] != '\0' ) { throw (rpr_int)RPR_ERROR_INTERNAL_ERROR; } // check that the last character is '\0'
		rprObjectName = std::string(frobjectName_data);
		delete[] frobjectName_data; frobjectName_data = NULL;
	}

	GiveUniqueNodeName(xmlNodeNames, "img", newNode.name );


	{
		RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
		newParam.name = "path";
		newParam.type = "file_path";

		if ( rprObjectName == "" )
		{
			newParam.value = "____ERROR_UNDEF_PATH____"; // make sure to call rprObjectSetName on all rpr_image , in order to avoir this error.
		}
		else
		{
			newParam.value = rprObjectName;
		}

		newNode.paramList.push_back(newParam);
	}

	{
		rpr_float imgGamma;
		status = rprImageGetInfo(imgNodeIn, RPR_IMAGE_GAMMA, sizeof(imgGamma), &imgGamma, NULL); MACRO_CHECK_RPR_STATUS;
		if ( imgGamma < 0.0 ) // we can have  imgGamma=-1, meaning the image is using gamma from context
		{
			imgGamma = 1.0f;
		}

		RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
		newParam.name = "gamma";
		newParam.type = "float";
		newParam.value = std::to_string( imgGamma );
		newNode.paramList.push_back(newParam);
	}

	nodesSet[imgNodeIn] = newNode;

	return;
}

void XMLExporter_fillMaterialList(
	rpr_material_node matNodeIn, 
	std::unordered_map<void*,RPRTOOLS_NODE_EXPORT_DEFINE>& nodesSet, 
	RPRStringIDMapper& strIdMapper,
	std::unordered_set<std::string>& xmlNodeNames
	)
{
	// if material already in export list
	if ( nodesSet.find(matNodeIn) != nodesSet.end() )
	{
		return;
	}

	rpr_int status = RPR_SUCCESS;

	RPRTOOLS_NODE_EXPORT_DEFINE newNode;
	newNode.rprMat = matNodeIn;

	rpr_material_node_type nodeType = (rpr_material_node_type)0;
	status = rprMaterialNodeGetInfo(matNodeIn,RPR_MATERIAL_NODE_TYPE, sizeof(nodeType),&nodeType,NULL); MACRO_CHECK_RPR_STATUS;
	strIdMapper.RPRMaterialType_id_to_string(nodeType,newNode.type);

	uint64_t nbInput = 0;
	status = rprMaterialNodeGetInfo(matNodeIn,RPR_MATERIAL_NODE_INPUT_COUNT, sizeof(nbInput),&nbInput,NULL); MACRO_CHECK_RPR_STATUS;

	std::string nameFromRPR;

	{
		size_t frobjectName_size = 0;
		status = rprMaterialNodeGetInfo(matNodeIn, RPR_MATERIAL_NODE_NAME, NULL, NULL, &frobjectName_size); MACRO_CHECK_RPR_STATUS;
		if ( frobjectName_size <= 0 ) { throw (rpr_int)RPR_ERROR_INTERNAL_ERROR; } // because of 0 terminated character, size must be >= 1
		char* frobjectName_data = new char[frobjectName_size];
		status = rprMaterialNodeGetInfo(matNodeIn, RPR_MATERIAL_NODE_NAME, frobjectName_size, frobjectName_data, NULL); MACRO_CHECK_RPR_STATUS;
		if ( frobjectName_data[frobjectName_size-1] != '\0' ) { throw (rpr_int)RPR_ERROR_INTERNAL_ERROR; } // check that the last character is '\0'
		nameFromRPR = std::string(frobjectName_data);
		delete[] frobjectName_data; frobjectName_data = NULL;
	}

	GiveUniqueNodeName(xmlNodeNames, nameFromRPR, newNode.name );

	for (int i = 0; i < nbInput; i++)
	{

		rpr_material_node_input paramNameID = (rpr_material_node_input)0;
		status = rprMaterialNodeGetInputInfo(matNodeIn, i, RPR_MATERIAL_NODE_INPUT_NAME, sizeof(paramNameID), &paramNameID, NULL);
			
		std::string paramName_;
		strIdMapper.RPRMaterialInput_id_to_string(paramNameID, paramName_);
		if ( paramName_ == "" )
		{
			throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
		}

		rpr_uint nodeInputType = 0;
		status = rprMaterialNodeGetInputInfo(matNodeIn, i, RPR_MATERIAL_NODE_INPUT_TYPE, sizeof(nodeInputType), &nodeInputType, NULL);
		MACRO_CHECK_RPR_STATUS;

		size_t shaderParameterValue_lenght_size_t = 0;
		status = rprMaterialNodeGetInputInfo(matNodeIn, i, RPR_MATERIAL_NODE_INPUT_VALUE, 0, NULL, &shaderParameterValue_lenght_size_t);
		uint64_t shaderParameterValue_lenght = shaderParameterValue_lenght_size_t;
		MACRO_CHECK_RPR_STATUS;
		char* shaderParameterValue = new char[shaderParameterValue_lenght];
		status = rprMaterialNodeGetInputInfo(matNodeIn, i, RPR_MATERIAL_NODE_INPUT_VALUE, shaderParameterValue_lenght, shaderParameterValue, NULL);
		MACRO_CHECK_RPR_STATUS;


		if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_NODE )
		{
			if (shaderParameterValue_lenght != sizeof(rpr_material_node))
			{
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			}
			else
			{
				

				rpr_material_node* newMat = (rpr_material_node*)shaderParameterValue;
				if (*newMat != NULL)
				{
					XMLExporter_fillMaterialList(*newMat, nodesSet, strIdMapper, xmlNodeNames);

					RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
					newParam.name = paramName_;
					newParam.type = "connection";
					newParam.value = nodesSet[*newMat].name;
					newNode.paramList.push_back(newParam);
				}
				MACRO_CHECK_RPR_STATUS;

			}
		}
		else if (nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_IMAGE)
		{
			if (shaderParameterValue_lenght != sizeof(rpr_image))
			{
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			}
			else
			{
				rpr_image* image = (rpr_image*)shaderParameterValue;
				if (*image != NULL)
				{
					XMLExporter_fillImageList(*image, nodesSet, strIdMapper, xmlNodeNames);

					RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
					newParam.name = paramName_;
					newParam.type = "connection";
					newParam.value = nodesSet[*image].name;
					newNode.paramList.push_back(newParam);
				}
				MACRO_CHECK_RPR_STATUS;
			}
		}
		else if (nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_BUFFER)
		{
			if (shaderParameterValue_lenght != sizeof(rpr_buffer))
			{
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			}
			else
			{
				rpr_buffer* buffer = (rpr_buffer*)shaderParameterValue;
				if (*buffer != NULL)
				{
					// TODO
				}
				MACRO_CHECK_RPR_STATUS;
			}
		}
		else if (nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_GRID)
		{
			if (shaderParameterValue_lenght != sizeof(rpr_grid))
			{
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			}
			else
			{
				rpr_grid* grid = (rpr_grid*)shaderParameterValue;
				if (*grid != NULL)
				{
					// TODO
				}
				MACRO_CHECK_RPR_STATUS;
			}
		}
		else if (nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_LIGHT)
		{
			if (shaderParameterValue_lenght != sizeof(rpr_light))
			{
				throw (rpr_int)RPR_ERROR_INTERNAL_ERROR;
			}
			else
			{
				rpr_light* light = (rpr_light*)shaderParameterValue;
				if (*light != NULL)
				{
					// TODO
				}
				MACRO_CHECK_RPR_STATUS;
			}
		}
		else if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_FLOAT4 )
		{
			const float* f4 = (const float*)shaderParameterValue;

			RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
			newParam.name = paramName_;
			newParam.type = "float4";
			newParam.value = std::to_string(f4[0]) + ", " + std::to_string(f4[1]) + ", " + std::to_string(f4[2]) + ", " + std::to_string(f4[3]) ;
			newNode.paramList.push_back(newParam);
		}
		else if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_UINT )
		{
			unsigned int* ui1 = (unsigned int*)shaderParameterValue;

			RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
			newParam.name = paramName_;
			newParam.type = "uint";
			newParam.value = std::to_string(ui1[0]);
			newNode.paramList.push_back(newParam);

		}
		else if ( nodeInputType == RPR_MATERIAL_NODE_INPUT_TYPE_DATA )
		{
			RPRTOOLS_NODE_EXPORT_DEFINE::RPRTOOLS_MATERIAL_PARAM_EXPORT_DEFINE newParam;
			newParam.name = paramName_;
			newParam.type = "data";
			newParam.value = "TODO_TYPE_DATA";
			newNode.paramList.push_back(newParam);
		}

		delete[] shaderParameterValue; shaderParameterValue = NULL;
	}

	nodesSet[matNodeIn] = newNode;

	return;
}


rpr_int rprtools_MaterialXMLExport(
	rpr_material_node masterMaterial, 
	rpr_material_node masterMaterial_displacement,  
	const char* xmlFilePath, 
	const char* materialName)
{
	try
	{
		RPRStringIDMapper strIdMapper;

		std::unordered_set<std::string> xmlNodeNames; // list of  RPRTOOLS_NODE_EXPORT_DEFINE::name .  this list is used to make sure we never have 2 nodes with same name.  so at the end of XMLExporter_fillMaterialList, we must have  xmlNodeNames.size == materialNodes.size.
		std::unordered_map<void*,RPRTOOLS_NODE_EXPORT_DEFINE> materialNodes;
		XMLExporter_fillMaterialList(masterMaterial, materialNodes, strIdMapper, xmlNodeNames);
		
		std::string displacementMasterMatName;
		if ( masterMaterial_displacement )
		{
			XMLExporter_fillMaterialList(masterMaterial_displacement, materialNodes, strIdMapper, xmlNodeNames);
			displacementMasterMatName = materialNodes[masterMaterial_displacement].name;
		}

		const RPRTOOLS_NODE_EXPORT_DEFINE& mainMatDef = materialNodes[masterMaterial];

		std::ofstream xmlFile;
		xmlFile.open(xmlFilePath , std::ios::trunc | std::ios::binary);
		
		xmlFile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		
		char rprVersion[128];
		sprintf(rprVersion,"0x%x",RPR_VERSION_MAJOR_MINOR_REVISION);

		xmlFile << "<material name=\"";
		xmlFile << materialName;
		xmlFile << "\" version_exporter=\"10\" version_rpr=\"";
		xmlFile << rprVersion;
		xmlFile << "\" closure_node=\"";
		xmlFile << mainMatDef.name;

		if ( displacementMasterMatName != "" )
		{
			xmlFile << "\" displacement_node=\"";
			xmlFile << displacementMasterMatName;
		}

		xmlFile << "\">\n";
		
		xmlFile << "    <description></description>\n";


		for(const auto& imat : materialNodes)
		{
			xmlFile << "    <node name=\"";
			xmlFile << imat.second.name;
			xmlFile << "\" type=\"";
			xmlFile << imat.second.type;
			xmlFile << "\">\n";

			for(const auto& iParam : imat.second.paramList)
			{
				xmlFile << "        <param name=\"";
				xmlFile << iParam.name;
				xmlFile << "\" type=\"";
				xmlFile << iParam.type;
				xmlFile << "\" value=\"";
				xmlFile << iParam.value;
				xmlFile << "\"/>\n";
			}

			xmlFile << "    </node>\n";
		}

		xmlFile << "</material>\n";

		xmlFile.close();
	}
	catch(std::exception& e)
	{
		return RPR_ERROR_INTERNAL_ERROR;
	}
	catch(rpr_int e)
	{
		return e;
	}

	return RPR_SUCCESS;
}


