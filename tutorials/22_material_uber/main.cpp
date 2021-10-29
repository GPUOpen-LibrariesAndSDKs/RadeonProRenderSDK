/*****************************************************************************\
*
*  Module Name    Material Ball scene with UBER materials
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
// This demo illustrates examples for the UBER ( RPR_MATERIAL_NODE_UBERV2 ) material. 
//


RPRGarbageCollector g_gc;



int main()
{

	//	for Debugging you can enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);


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

	MatballScene matballScene;

	MatballScene::MATBALL matBall0 = matballScene.Init(context,0,0);

	{ 
		rpr_material_node material = nullptr;

		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_UBERV2, &material));
		g_gc.GCAdd(material);

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR, 0.5f, 0.25f, 0.0f, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT, 1.f, 1.f, 1.f, 1.f));

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, 0.5f, 0.5f, 0.5f, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT, 0.9f, 0.9f, 0.9f, 1.f));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, 0.1f, 0.f, 0.f, 0.f));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE, RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_METALNESS, 0.f, 0.f, 0.f, 1.f));

		matBall0.SetMaterial(material);

		matballScene.Render("22_material_uber_00.png");

	}
	


	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(+1,0);

		rpr_material_node material = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_UBERV2, &material));
		g_gc.GCAdd(material);

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS   ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,0.996078, 0.858824, 0.639216, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR   ,0.996078, 0.858824, 0.639216, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT    ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR    ,1.36, 1.36, 1.36, 1.36));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE   ,0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR    ,0.996078, 0.858824, 0.639216, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS   ,0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_COLOR  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_MODE     ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_IOR     ,3, 3, 3, 3));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_METALNESS   ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR     ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN    ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN_TINT   ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_MODE    ,RPR_UBER_MATERIAL_EMISSION_MODE_SINGLESIDED));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_TRANSPARENCY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT   ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER    ,0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR   ,1, 1, 1, 1));

		matBall.SetMaterial(material);
		matballScene.CameraLook1Shape(+1,0);
		matballScene.Render("22_material_uber_01.png");
	}

	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,0);

		rpr_material_node material = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_UBERV2, &material));
		g_gc.GCAdd(material);


		rpr_image uberMat3_img = nullptr;
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/brass standard_bump.png",&uberMat3_img));
		g_gc.GCAdd(uberMat3_img);

		rpr_material_node uberMat3_imgTexture = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat3_imgTexture));
		g_gc.GCAdd(uberMat3_imgTexture);
		CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat3_imgTexture,   RPR_MATERIAL_INPUT_DATA  ,uberMat3_img));

		rpr_material_node uberMat3_bumpmap = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_BUMP_MAP,&uberMat3_bumpmap));
		g_gc.GCAdd(uberMat3_bumpmap);
		CHECK(rprMaterialNodeSetInputNByKey(uberMat3_bumpmap,   RPR_MATERIAL_INPUT_COLOR  ,uberMat3_imgTexture));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat3_bumpmap,   RPR_MATERIAL_INPUT_SCALE  ,0.2, 0.2, 0.2, 0.2));

		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR  ,0.937255, 0.870588, 0.576471, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT   ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS  ,0.5, 0.5, 0.5, 0.5));
		CHECK(rprMaterialNodeSetInputNByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL ,uberMat3_bumpmap));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR    ,0.937255, 0.870588, 0.576471, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT     ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS   ,0.1, 0.1, 0.1, 0.1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION   ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE    ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR    ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputNByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL   ,uberMat3_bumpmap));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR   ,1.5, 1.5, 1.5, 1.5));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE   ,0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR  ,1, 1, 1, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE   ,1, 1, 1, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS    ,1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_COLOR    ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS   ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_MODE     ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_IOR     ,3, 3, 3, 3));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_METALNESS     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN     ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN_TINT     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR     ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_EMISSION_MODE     ,RPR_UBER_MATERIAL_EMISSION_MODE_SINGLESIDED));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_TRANSPARENCY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT      ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER   ,0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR    ,0.937255, 0.870588, 0.576471, 0));


		matBall.SetMaterial(material);
		matballScene.CameraLook1Shape(-1,0);
		matballScene.Render("22_material_uber_02.png");
	}


	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,-1);

		rpr_material_node material = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_UBERV2, &material));
		g_gc.GCAdd(material);

		rpr_image image_0x000001B218D09B60 = nullptr;
		CHECK( rprContextCreateImageFromFile(context,"../../Resources/Textures/CarbonFiber_N.jpg",&image_0x000001B218D09B60));
		g_gc.GCAdd(image_0x000001B218D09B60);

		rpr_material_node materialnode_0x000001B218D0A110 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&materialnode_0x000001B218D0A110));
		g_gc.GCAdd(materialnode_0x000001B218D0A110);
		CHECK( rprMaterialNodeSetInputImageDataByKey(materialnode_0x000001B218D0A110,RPR_MATERIAL_INPUT_DATA,image_0x000001B218D09B60));

		rpr_material_node materialnode_0x000001B218D0A1E0 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_NORMAL_MAP,&materialnode_0x000001B218D0A1E0));
		g_gc.GCAdd(materialnode_0x000001B218D0A1E0);
		CHECK( rprMaterialNodeSetInputFByKey(materialnode_0x000001B218D0A1E0,RPR_MATERIAL_INPUT_SCALE,3.50f,3.50f,1.0f,1.0f));
		CHECK( rprMaterialNodeSetInputNByKey(materialnode_0x000001B218D0A1E0,RPR_MATERIAL_INPUT_COLOR,materialnode_0x000001B218D0A110));


		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,1.0f,1.0f,1.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,0.4790f,0.4790f,0.4790f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputNByKey(material,RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL,materialnode_0x000001B218D0A1E0));

		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,1.0f,1.0f,1.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,1.0f,1.0f,1.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,0.6500f,0.6500f,0.6500f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputUByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,0));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.50f,1.50f,1.50f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputNByKey(material,RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL,materialnode_0x000001B218D0A1E0));

		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_TRANSPARENCY,0.0f,0.0f,0.0f,0.0f));
		
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR,1.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE,0.5f,0.5f,0.5f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION,0.0f,0.0f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT,0.8720f,0.8720f,0.8720f,0.0f));
		CHECK( rprMaterialNodeSetInputUByKey(material,RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER,1));
		
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR,1.0f,0.70f,0.0f,0.0f));
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT,0.7500f,0.7500f,0.7500f,0.0f));
		
		CHECK( rprMaterialNodeSetInputFByKey(material,RPR_MATERIAL_INPUT_SCHLICK_APPROXIMATION,1.0f,1.0f,1.0f,1.0f));
		
	
		matBall.SetMaterial(material);
		matballScene.CameraLook1Shape(-1,-1);
		matballScene.Render("22_material_uber_03.png", 500); // for better quality on SSS close look, it's better to increase iteration count
	}

	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-1);

		rpr_image uberMat2_img1 = nullptr;
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Base_Color.jpg",&uberMat2_img1));
		g_gc.GCAdd(uberMat2_img1);

		rpr_image uberMat2_img2 = nullptr;
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/lead_rusted_Normal.jpg",&uberMat2_img2));
		g_gc.GCAdd(uberMat2_img2);
	
		rpr_material_node uberMat2_imgTexture1 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture1));
		g_gc.GCAdd(uberMat2_imgTexture1);
		CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture1,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img1));

		rpr_material_node uberMat2_imgTexture2 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture2));
		g_gc.GCAdd(uberMat2_imgTexture2);
		CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture2,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img2));


		rpr_material_node matNormalMap = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_NORMAL_MAP,&matNormalMap));
		g_gc.GCAdd(matNormalMap);
		CHECK( rprMaterialNodeSetInputFByKey(matNormalMap,RPR_MATERIAL_INPUT_SCALE,1.0f,1.0f,1.0f,1.0f));
		CHECK( rprMaterialNodeSetInputNByKey(matNormalMap,RPR_MATERIAL_INPUT_COLOR,uberMat2_imgTexture2));


		rpr_material_node uberMat2 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat2));
		g_gc.GCAdd(uberMat2);

		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,uberMat2_imgTexture1));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL   ,matNormalMap));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,1, 1, 1, 1));

		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,uberMat2_imgTexture1));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL   ,matNormalMap));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));

		// Apply this new Uber Material to the shapes
		matBall.SetMaterial(uberMat2);
		matballScene.CameraLook1Shape(0,-1);
		matballScene.Render("22_material_uber_04.png");
	}

	//
	// Demo of a Subdiv / Displacement material
	//
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball( +1, -1, 
			false // Displacement doesn't work on instanced mesh
			);

		rpr_material_node uberMat = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat));
		CHECK(rprObjectSetName(uberMat,"Uber_0"));
		g_gc.GCAdd(uberMat);
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,0.501961f,0.501961f,0.501961f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.330000f,1.330000f,1.330000f,1.330000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR,0.501961f,0.898039f,0.996078f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR,1.330000f,1.330000f,1.330000f,1.330000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR,0.501961f,0.898039f,0.996078f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_IOR,3.0f,3.0f,3.0f,3.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_TINT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_TRANSPARENCY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR,1.0f,1.0f,1.0f,1.0f));

		rpr_material_node materialnode_0x000001AF2F01FA60 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&materialnode_0x000001AF2F01FA60));
		g_gc.GCAdd(materialnode_0x000001AF2F01FA60);
		CHECK(rprObjectSetName(materialnode_0x000001AF2F01FA60,(rpr_char*)"node0"));

		rpr_image image_0x000001AF2F01FCD0 = nullptr;
		CHECK(rprContextCreateImageFromFile(context, "../../Resources/Textures/Water_Noise_Displacement.tga", &image_0x000001AF2F01FCD0));
		g_gc.GCAdd(image_0x000001AF2F01FCD0);
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialnode_0x000001AF2F01FA60, RPR_MATERIAL_INPUT_DATA, image_0x000001AF2F01FCD0));


		

		rpr_shape iShape[] = { matBall.base,  matBall.inner, matBall.outer };
		for(const auto& i : iShape )
		{
			CHECK(rprShapeSetSubdivisionFactor(i,1));
			CHECK(rprShapeSetSubdivisionCreaseWeight(i,0.0f));
			CHECK(rprShapeSetSubdivisionBoundaryInterop(i,RPR_SUBDIV_BOUNDARY_INTERFOP_TYPE_EDGE_ONLY));
			CHECK(rprShapeSetDisplacementScale(i, 0.0f, 0.004f));
			CHECK(rprShapeSetDisplacementMaterial(i,materialnode_0x000001AF2F01FA60));
		}

		matBall.SetMaterial(uberMat);
		
		matballScene.CameraLook1Shape(+1,-1);
		matballScene.Render("22_material_uber_05.png");
	}

	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,-2);


		rpr_material_node uberMat = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat));
		CHECK(rprObjectSetName(uberMat,"Uber_0"));
		g_gc.GCAdd(uberMat);
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,0.270588f,0.270588f,0.270588f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,0.500000f,0.500000f,0.500000f,0.500000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,0.0f,0.313726f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,2));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR,1.500000f,1.500000f,1.500000f,1.500000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_IOR,3.0f,3.0f,3.0f,3.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_TINT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_TRANSPARENCY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR,0.270588f,0.270588f,0.270588f,0.0f));


		rpr_image image_0x000001B091A198E0 = nullptr;
		//ImageFromFile creation : original path : "../unittestdata/matlib/RadeonProRMaps/Aluminium Anodized roughness.png"
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/Aluminium Anodized roughness.png",&image_0x000001B091A198E0));
		g_gc.GCAdd(image_0x000001B091A198E0);

		rpr_material_node materialnode_0x000001B091A199B0 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&materialnode_0x000001B091A199B0));
		g_gc.GCAdd(materialnode_0x000001B091A199B0);
		CHECK(rprObjectSetName(materialnode_0x000001B091A199B0,"node0"));

		rpr_material_node materialnode_0x000001B091A19A80 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_BUMP_MAP,&materialnode_0x000001B091A19A80));
		g_gc.GCAdd(materialnode_0x000001B091A19A80);
		CHECK(rprObjectSetName(materialnode_0x000001B091A19A80,"node1"));
		CHECK(rprMaterialNodeSetInputFByKey(materialnode_0x000001B091A19A80,RPR_MATERIAL_INPUT_SCALE,0.010000f,0.010000f,0.010000f,0.010000f));
		
		rpr_material_node materialnode_0x000001B091A1AE00 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&materialnode_0x000001B091A1AE00));
		g_gc.GCAdd(materialnode_0x000001B091A1AE00);
		CHECK(rprObjectSetName(materialnode_0x000001B091A1AE00,"node2"));

		rpr_image image_0x000001B091A19810 = nullptr;
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/aluminium anodized_bump.png",&image_0x000001B091A19810));
		g_gc.GCAdd(image_0x000001B091A19810);
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialnode_0x000001B091A199B0,RPR_MATERIAL_INPUT_DATA,image_0x000001B091A19810));
		CHECK(rprMaterialNodeSetInputNByKey(materialnode_0x000001B091A19A80,RPR_MATERIAL_INPUT_COLOR,materialnode_0x000001B091A199B0));
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialnode_0x000001B091A1AE00,RPR_MATERIAL_INPUT_DATA,image_0x000001B091A198E0));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL,materialnode_0x000001B091A19A80));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,materialnode_0x000001B091A1AE00));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL,materialnode_0x000001B091A19A80));


		matBall.SetMaterial(uberMat);
		matballScene.CameraLook1Shape(-1,-2);
		matballScene.Render("22_material_uber_06.png");
	}

	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-2);


		rpr_material_node uberMat = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat));
		g_gc.GCAdd(uberMat);
		CHECK(rprObjectSetName(uberMat,"Uber_0"));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,0.501961f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,0.500000f,0.500000f,0.500000f,0.500000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,0.490196f,0.490196f,0.490196f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,0.990000f,0.990000f,0.990000f,0.990000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,0.008000f,0.008000f,0.008000f,0.008000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.460000f,1.460000f,1.460000f,1.460000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR,1.500000f,1.500000f,1.500000f,1.500000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_COLOR,0.490196f,0.490196f,0.490196f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS,0.008000f,0.008000f,0.008000f,0.008000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_IOR,1.460000f,1.460000f,1.460000f,1.460000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR,0.0f,0.0f,0.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_TINT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_TRANSPARENCY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR,0.501961f,0.0f,0.0f,0.0f));


		matBall.SetMaterial(uberMat);
		matballScene.CameraLook1Shape(0,-2);
		matballScene.Render("22_material_uber_07.png");
	}

	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(+1,-2);

	
		rpr_material_node uberMat = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat));
		g_gc.GCAdd(uberMat);
		CHECK(rprObjectSetName(uberMat,"Uber_0"));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR,0.752941f,0.596078f,0.443137f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS,0.500000f,0.500000f,0.500000f,0.500000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR,0.666667f,0.490196f,0.313726f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS,0.300000f,0.300000f,0.300000f,0.300000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE,2));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR,1.500000f,1.500000f,1.500000f,1.500000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE,1.0f,1.0f,1.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_COLOR,0.752941f,0.596078f,0.443137f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS,0.420000f,0.420000f,0.420000f,0.420000f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_IOR,1.700000f,1.700000f,1.700000f,1.700000f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_METALNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR,0.0f,0.0f,0.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_TINT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR,1.0f,1.0f,1.0f,1.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_EMISSION_MODE,1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_TRANSPARENCY,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat,RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER,0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT,0.0f,0.0f,0.0f,0.0f));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat,RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR,0.752941f,0.596078f,0.443137f,0.0f));


		rpr_material_node materialnode_0x000001B091A1A2A0 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_BUMP_MAP,&materialnode_0x000001B091A1A2A0));
		g_gc.GCAdd(materialnode_0x000001B091A1A2A0);
		CHECK(rprObjectSetName(materialnode_0x000001B091A1A2A0,"node0"));
		CHECK(rprMaterialNodeSetInputFByKey(materialnode_0x000001B091A1A2A0,RPR_MATERIAL_INPUT_SCALE,0.900000f,0.900000f,0.900000f,0.900000f));
		
		rpr_material_node materialnode_0x000001B091A1A6B0 = nullptr;
		CHECK(rprMaterialSystemCreateNode(matballScene.m_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&materialnode_0x000001B091A1A6B0));
		g_gc.GCAdd(materialnode_0x000001B091A1A6B0);
		CHECK(rprObjectSetName(materialnode_0x000001B091A1A6B0,"node1"));



		rpr_image image_0x000001B091A19F60 = nullptr;
		//ImageFromFile creation : original path : "../unittestdata/matlib/RadeonProRMaps/brass polished.jpg"
		CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/brass polished.jpg",&image_0x000001B091A19F60));
		g_gc.GCAdd(image_0x000001B091A19F60);
		CHECK(rprMaterialNodeSetInputNByKey(materialnode_0x000001B091A1A2A0,RPR_MATERIAL_INPUT_COLOR,materialnode_0x000001B091A1A6B0));
		CHECK(rprMaterialNodeSetInputImageDataByKey(materialnode_0x000001B091A1A6B0,RPR_MATERIAL_INPUT_DATA,image_0x000001B091A19F60));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL,materialnode_0x000001B091A1A2A0));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL,materialnode_0x000001B091A1A2A0));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat,RPR_MATERIAL_INPUT_UBER_COATING_NORMAL,materialnode_0x000001B091A1A2A0));
		//CHECK(rprEnvironmentLightSetIntensityScale(light_0x000001AF2F01FE70,1.0f));


		matBall.SetMaterial(uberMat);
		matballScene.CameraLook1Shape(+1,-2);
		matballScene.Render("22_material_uber_08.png");
	}


	//
	//
	// If needed, more material ball can be added in the future
	//
	//


	// display the final scene : camera zoomed-out
	matballScene.CameraLook9Shape();
	matballScene.Render("22_material_uber_final.png",200);

	// clean everything
	g_gc.GCClean();
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}





