/*****************************************************************************\
*
*  Module Name    Material Ball scene with basic materials
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
// This demo illustrates how to create materials in RPR.
// It's not covering the UBER material which is used inside the 22_material_uber demo.
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


	// demo for a Layered material : blending a DIFFUSE and a MICROFACET
	{ 
		rpr_material_node base = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_DIFFUSE, &base));
		g_gc.GCAdd(base);
		CHECK(  rprMaterialNodeSetInputFByKey(base, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.25f, 0.f, 1.f)); // Diffuse color


		rpr_material_node top = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MICROFACET, &top));
		g_gc.GCAdd(top);
		CHECK(  rprMaterialNodeSetInputFByKey(top, RPR_MATERIAL_INPUT_COLOR, 1.f, 1.f, 1.f, 1.f)); // Specular color
		CHECK(  rprMaterialNodeSetInputFByKey(top, RPR_MATERIAL_INPUT_ROUGHNESS, 0.05f, 0.f, 0.f, 1.f)); // Roughness


		rpr_material_node fresnel = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_FRESNEL, &fresnel)); 
		g_gc.GCAdd(fresnel);
		CHECK(  rprMaterialNodeSetInputFByKey(fresnel, RPR_MATERIAL_INPUT_IOR, 1.4f,0.0f,0.0f,0.0f )); 


		// Create layered material
		rpr_material_node layered = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_BLEND, &layered));
		g_gc.GCAdd(layered);
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR0, base)); // Set material for base layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR1, top)); // Set material for top layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_WEIGHT, fresnel));


		matBall0.SetMaterial(layered);

		matballScene.Render("21_material_00.png");

	}
	

	// demo for the reflection material
	{
		MatballScene::MATBALL matBall1 = matballScene.AddMatball(+1,0);

		rpr_material_node material = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_REFLECTION, &material));
		g_gc.GCAdd(material);

		matBall1.SetMaterial(material);

		matballScene.CameraLook1Shape(+1,0);

		matballScene.Render("21_material_01.png");
	}


	// demo for procedural material
	{ 
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,0);


		rpr_material_node diffuseA = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseA));
		g_gc.GCAdd(diffuseA);
		CHECK(  rprMaterialNodeSetInputFByKey(diffuseA, RPR_MATERIAL_INPUT_COLOR, 0.0f, 0.5f, 1.0f, 0.0f)); // Diffuse color

		rpr_material_node diffuseB = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseB));
		g_gc.GCAdd(diffuseB);
		CHECK(  rprMaterialNodeSetInputFByKey(diffuseB, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.20f, 1.0f, 0.0f)); // Diffuse color

		rpr_material_node uv_node = NULL;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node));
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV));


		rpr_material_node uv_scaled_node = NULL;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node));
		g_gc.GCAdd(uv_scaled_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL));
		CHECK(  rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node));
		CHECK( rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 0.7f, 0.7f, 0.0f, 0));


		rpr_material_node checker = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_CHECKER_TEXTURE, &checker)); 
		g_gc.GCAdd(checker);
		CHECK(  rprMaterialNodeSetInputNByKey(checker, RPR_MATERIAL_INPUT_UV  , uv_scaled_node));


		// Create layered material
		rpr_material_node layered = nullptr;
		CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_BLEND, &layered));
		g_gc.GCAdd(layered);
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR0, diffuseA)); // Set material for base layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_COLOR1, diffuseB)); // Set material for top layer
		CHECK(  rprMaterialNodeSetInputNByKey(layered, RPR_MATERIAL_INPUT_WEIGHT, checker));


		matBall.SetMaterial(layered);

		matballScene.CameraLook1Shape(-1,0);

		matballScene.Render("21_material_02.png");

	}



	//
	// There are free slots left for more material demos.
	// They may be used in the future with SDK updates.
	//


	// display the final scene : camera zoomed-out
	matballScene.CameraLook9Shape();
	matballScene.Render("21_material_final.png");

	// clean everything
	g_gc.GCClean();
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}



