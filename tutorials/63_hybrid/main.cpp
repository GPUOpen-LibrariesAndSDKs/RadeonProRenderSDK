/*****************************************************************************\
*
*  Module Name    Hybrid Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/


//
//
//	This is a demo for the Hybrid.DLL plugin.
//
//


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
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK matball scene rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create RPR context using a single GPU 
	rpr_context context = nullptr;
	
	std::string hybridPluginName = 
	#if defined(WIN32)
	"HybridPro.dll";
	#elif defined(__LINUX__)
	"HybridPro.so";
	#elif defined(__APPLE__)
	""; // no Hybrid plugin released on MacOS
	#endif
	
	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(hybridPluginName.c_str());
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	rpr_status ctxCreateStatus = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context);
	if ( ctxCreateStatus != RPR_SUCCESS )
	{
		std::cout << "Error creating context - maybe the GPU is not compatible with " << hybridPluginName << "\n" ;
		return 0;
	}

	// Set active plugin.
	CHECK(rprContextSetActivePlugin(context, plugins[0]));

	MatballScene matballScene;
	MatballScene::MATBALL matBall0 = matballScene.Init(context,0,0,true);

	// flip Y, otherwise out image is reversed.
	CHECK(rprContextSetParameterByKey1u(context, RPR_CONTEXT_Y_FLIP, 1));


	//
	// Create a new Uber material for the matball model.
	// note: Hybrid only manages UBER material ( meaning we can't use  RPR_MATERIAL_NODE_DIFFUSE, RPR_MATERIAL_NODE_MICROFACET... like with Northstar )
	//

	rpr_image uberMat2_img1 = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Base_Color.jpg",&uberMat2_img1));
	rpr_image uberMat2_img2 = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Normal.jpg",&uberMat2_img2));
	
	rpr_material_node uberMat2_imgTexture1 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture1));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture1,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img1));

	rpr_material_node uberMat2_imgTexture2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture2));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture2,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img2));

	rpr_material_node matNormalMap = nullptr;
	CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_NORMAL_MAP,&matNormalMap));
	CHECK( rprMaterialNodeSetInputNByKey(matNormalMap,RPR_MATERIAL_INPUT_COLOR,uberMat2_imgTexture2));

	rpr_material_node uberMat2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat2));

	CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,uberMat2_imgTexture1));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL   ,matNormalMap));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,1, 1, 1, 1));
	
	CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,uberMat2_imgTexture1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));

	// Apply this new Uber Material to the shapes
	matBall0.SetMaterial(uberMat2);


	int iterationCount = 100;


	// if using Hybrid ( not HybridPro ) then disable iteration count
	if ( hybridPluginName.find("Hybrid.") != std::string::npos )
		iterationCount = -1;


	// rendering.
	matballScene.Render("63.png",
		iterationCount,
		false // for Hybrid and HybridPro , no need of framebuffer resolve
		);

	// Clean
	CHECK(rprObjectDelete(uberMat2_img1));uberMat2_img1=nullptr;
	CHECK(rprObjectDelete(uberMat2_img2));uberMat2_img2=nullptr;
	CHECK(rprObjectDelete(matNormalMap));matNormalMap=nullptr;
	CHECK(rprObjectDelete(uberMat2_imgTexture1));uberMat2_imgTexture1=nullptr;
	CHECK(rprObjectDelete(uberMat2_imgTexture2));uberMat2_imgTexture2=nullptr;
	CHECK(rprObjectDelete(uberMat2));uberMat2=nullptr;
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;

	return 0;

}

