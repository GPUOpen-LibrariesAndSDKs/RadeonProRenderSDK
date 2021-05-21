/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
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


/*

	This is a demo for the Hybrid.DLL plugin.

*/

#include "RadeonProRender.h"
#include "RadeonProRender_Baikal.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Import
#include "../common/common.h"

#include <cassert>
#include <iostream>


int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	frContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK matball scene rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create RPR context using a single GPU 
	rpr_context context = nullptr;
	
	std::string hybridPluginName = 
	#if defined(WIN32)
	"Hybrid.dll";
	#elif defined(__LINUX__)
	"Hybrid.so";
	#elif defined(__APPLE__)
	""; // no Hybrid plugin released on MacOS
	#endif
	
	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(hybridPluginName.c_str());
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

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK(rprContextCreateScene(context, &scene));
	// Set scene to render for the context
	CHECK(rprContextSetScene(context, scene));

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 1000, 1000 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));

	/////////Mesh Import//////////
	CHECK(rprsImport("../../Resources/Meshes/matball.rprs", context, matsys, &scene, true));

	//
	// parse the shapes from matball.rprs : we are going to change the material assigned on each shapes.
	//

	std::vector<rpr_shape> matBallShape;
	rpr_shape planeShape = nullptr;
	{
		int nbOfObjs = 0;
		CHECK(rprsListImportedShapes(0,0,&nbOfObjs)); // list the shapes loaded by last rprsImport call
		rpr_shape* objs = new rpr_shape[nbOfObjs];
		CHECK(rprsListImportedShapes(objs,nbOfObjs*sizeof(rpr_shape),0));
		for(int i=0; i<nbOfObjs; i++)
		{
			char name[2048];
			CHECK(rprShapeGetInfo(objs[i], RPR_SHAPE_NAME, sizeof(name), name, nullptr));

			if (   strcmp(name,"Probe01") == 0
				|| strcmp(name,"Probe02") == 0
				|| strcmp(name,"Probe00") == 0
				)
			{
				matBallShape.push_back(objs[i]);
			}
			else if ( strcmp(name,"BasePlaneOrb") == 0 )
			{
				planeShape = objs[i];
			}
		}
		delete[] objs; objs = NULL;
	}


	//
	// create a new Uber material for the floor
	// ( this is because Hybrid doesn't support RPR_MATERIAL_NODE_DIFFUSE, which is the material used on the floor in the original matball.rprs file )
	//

	rpr_image uberMat3_img = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/amd.png",&uberMat3_img));

	rpr_material_node uberMat3_imgTexture = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat3_imgTexture));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat3_imgTexture,   RPR_MATERIAL_INPUT_DATA  ,uberMat3_img));

	rpr_material_node uberMat3 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat3));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR  ,uberMat3_imgTexture));

	CHECK(rprShapeSetMaterial(planeShape,uberMat3));


	//
	// Create a new Uber material for the orb model.
	//

	rpr_image uberMat2_img1 = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Base_Color.jpg",&uberMat2_img1));
	rpr_image uberMat2_img2 = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Normal.jpg",&uberMat2_img2));
	
	rpr_material_node uberMat2_imgTexture1 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture1));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture1,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img1));
	rpr_material_node uberMat2_imgTexture2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture2));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture2,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img2));

	rpr_material_node uberMat2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat2));

	CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,uberMat2_imgTexture1));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL   ,uberMat2_imgTexture2));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,1, 1, 1, 1));
	
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));

	// Apply this new Uber Material to the shapes
	for(const auto& iShape : matBallShape)
	{
		CHECK(rprShapeSetMaterial(iShape,uberMat2));
	}



	std::cout << "Rendering scene ... ";
	CHECK(rprContextRender(context));
	std::cout << "finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer, "63.png"));



	//
	// Cleaning
	//

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(uberMat3_img));uberMat3_img=nullptr;
	CHECK(rprObjectDelete(uberMat3_imgTexture));uberMat3_imgTexture=nullptr;
	CHECK(rprObjectDelete(uberMat3));uberMat3=nullptr;
	CHECK(rprObjectDelete(uberMat2));uberMat2=nullptr;
	CHECK(rprObjectDelete(uberMat2_img1));uberMat2_img1=nullptr;
	CHECK(rprObjectDelete(uberMat2_img2));uberMat2_img2=nullptr;
	CHECK(rprObjectDelete(uberMat2_imgTexture1));uberMat2_imgTexture1=nullptr;
	CHECK(rprObjectDelete(uberMat2_imgTexture2));uberMat2_imgTexture2=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

