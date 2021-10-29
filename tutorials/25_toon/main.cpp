/*****************************************************************************\
*
*  Module Name    Toon Material Demo
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
// This demo illustrates how to use the Toon material.
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
	MatballScene::MATBALL matBall0 = matballScene.Init(context,-1,-1);

	// for the Toon Material Demo, it's better to use linear rendering
	CHECK( rprContextSetParameterByKey1f(matballScene.m_context, RPR_CONTEXT_DISPLAY_GAMMA , 1.0f ) );

	// remove env light, and replace it by a simple directional light.
	// This is better for Toon renderings.
	CHECK( rprSceneSetEnvironmentLight(matballScene.m_scene,nullptr) );
	rpr_light light=nullptr;
	{
		CHECK(rprContextCreateDirectionalLight(context, &light));
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0.0f,0.0f,0.0f)) * RadeonProRender::rotation_y(0.5f*3.14f/2.0f);
		CHECK(rprLightSetTransform(light, true, &lightm.m00));
		CHECK(rprDirectionalLightSetRadiantPower3f(light, 1.5f, 1.5f, 1.5f));
		CHECK(rprSceneAttachLight(matballScene.m_scene, light));
	}


	// 3 tones toon
	{
		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1, 0.7f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2, 0.9f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.0f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.0f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 199.0f/255.0f, 230.0f/255.0f, 116.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID,       112.0f/255.0f, 176.0f/255.0f,  75.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,     46.0f/255.0f,  88.0f/255.0f,  76.0f/255.0f,     0.f));
	
		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_NONE));

		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		matBall0.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(-1,-1);
		matballScene.Render("25_toon_00.png");
	}
	

	// Basic toon
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-1);

		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1, 0.6f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2, 0.9f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.0f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.0f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 246.0f/255.0f, 235.0f/255.0f, 171.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID,       246.0f/255.0f, 235.0f/255.0f, 171.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,    217.0f/255.0f, 146.0f/255.0f,  96.0f/255.0f,     0.f));
	
		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_NONE));


		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		matBall.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(0,-1);
		matballScene.Render("25_toon_01.png");
	}


	// 3D toon (using linear interpolation)
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(1,-1);

		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1, 0.6f,     0.0f, 0.0f, 0.f));  // limit between Shadow   and Mid
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2, 0.95f,     0.0f, 0.0f, 0.f)); // limit between Highlight and Mid

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between highlight and mid
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between mid and shadow
	
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 199.0f/255.0f, 230.0f/255.0f, 116.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID,       112.0f/255.0f, 176.0f/255.0f,  75.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,     46.0f/255.0f,  88.0f/255.0f,  76.0f/255.0f,     0.f));
	
		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_LINEAR));

		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		matBall.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(1,-1);
		matballScene.Render("25_toon_02.png");
	}


	
	// Simple toon with linear interpolation
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,-2);

		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1, 0.7f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2, 0.9f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.0f,     0.0f, 0.0f, 0.f)); // interp range between highlight and mid
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.2f,     0.0f, 0.0f, 0.f)); // interp range between mid and shadow
	
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 140.0f/255.0f, 209.0f/255.0f, 241.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID      , 140.0f/255.0f, 209.0f/255.0f, 241.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,     90.0f/255.0f, 120.0f/255.0f, 167.0f/255.0f,     0.f));
	
		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_LINEAR));

		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		matBall.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(-1,-2);
		matballScene.Render("25_toon_03.png");
	}


	// 5 colors toon shading ( without interpolation )
	// SHADOW2 -> SHADOW -> MID -> HIGHLIGHT -> HIGHLIGHT2
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-2);

		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_NONE));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW2   , 43.0f/255.0f   , 21.0f/255.0f, 34.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,     186.0f/255.0f,  97.0f/255.0f,  115.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID,       228.0f/255.0f, 167.0f/255.0f,  183.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 229.0f/255.0f, 220.0f/255.0f, 184.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT2, 236.0f/255.0f, 220.0f/255.0f, 223.0f/255.0f,     0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between highlight and mid
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between mid and shadow

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION_HIGHLIGHT  , 0.9999f,     0.0f, 0.0f, 0.f)); // limit HIGHLIGHT/HIGHLIGHT2
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2,           0.95f,     0.0f, 0.0f, 0.f));    // limit MID/HIGHLIGHT
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1,            0.6f,     0.0f, 0.0f, 0.f));    // limit SHADOW/MID
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION_SHADOW     , 0.4f,     0.0f, 0.0f, 0.f));    // limit SHADOW2/SHADOW
	
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE_SHADOW, 0.1f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE_HIGHLIGHT, 0.1f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_TOON_5_COLORS, 1));


		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		// demonstrate the effect of disable Shadow visibility on 1 matball
		CHECK( rprShapeSetVisibilityFlag(matBall.base, RPR_SHAPE_VISIBILITY_SHADOW, false));
		CHECK( rprShapeSetVisibilityFlag(matBall.inner, RPR_SHAPE_VISIBILITY_SHADOW, false));
		CHECK( rprShapeSetVisibilityFlag(matBall.outer, RPR_SHAPE_VISIBILITY_SHADOW, false));

		matBall.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(0,-2);
		matballScene.Render("25_toon_04.png");
	}



	// 5 colors toon shading ( with linear interpolation )
	// SHADOW2 -> SHADOW -> MID -> HIGHLIGHT -> HIGHLIGHT2
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(+1,-2);

		rpr_material_node toonRamp = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_RAMP, &toonRamp));
		g_gc.GCAdd(toonRamp);

		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_INTERPOLATION, RPR_INTERPOLATION_MODE_LINEAR));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW2   , 43.0f/255.0f   , 21.0f/255.0f, 34.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_SHADOW,     186.0f/255.0f,  97.0f/255.0f,  115.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_MID,       228.0f/255.0f, 167.0f/255.0f,  183.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT, 229.0f/255.0f, 220.0f/255.0f, 184.0f/255.0f,     0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_HIGHLIGHT2, 236.0f/255.0f, 220.0f/255.0f, 223.0f/255.0f,     0.f));

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE2, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between highlight and mid
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE1, 0.1f,     0.0f, 0.0f, 0.f)); // interp range between mid and shadow

		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION_HIGHLIGHT  , 0.9999f,     0.0f, 0.0f, 0.f)); // limit HIGHLIGHT/HIGHLIGHT2
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION2,           0.95f,     0.0f, 0.0f, 0.f));    // limit MID/HIGHLIGHT
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION1,            0.6f,     0.0f, 0.0f, 0.f));    // limit SHADOW/MID
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_POSITION_SHADOW     , 0.4f,     0.0f, 0.0f, 0.f));    // limit SHADOW2/SHADOW
	
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE_SHADOW, 0.1f,     0.0f, 0.0f, 0.f));
		CHECK( rprMaterialNodeSetInputFByKey(toonRamp,RPR_MATERIAL_INPUT_RANGE_HIGHLIGHT, 0.1f,     0.0f, 0.0f, 0.f));

		CHECK( rprMaterialNodeSetInputUByKey(toonRamp,RPR_MATERIAL_INPUT_TOON_5_COLORS, 1));

		rpr_material_node toonClosure = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_TOON_CLOSURE, &toonClosure));
		g_gc.GCAdd(toonClosure);

		CHECK( rprMaterialNodeSetInputFByKey(toonClosure,RPR_MATERIAL_INPUT_COLOR, 1.0f, 1.0f, 1.0f, 0.f));
		CHECK( rprMaterialNodeSetInputNByKey(toonClosure,RPR_MATERIAL_INPUT_DIFFUSE_RAMP, toonRamp));

		matBall.SetMaterial(toonClosure);
		matballScene.CameraLook1Shape(+1,-2);
		matballScene.Render("25_toon_05.png");
	}


	// display the final scene : camera zoomed-out
	matballScene.CameraLook9Shape();
	matballScene.Render("25_toon_final.png");

	// clean everything
	CHECK(rprObjectDelete(light)); light=nullptr;
	g_gc.GCClean();
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
 	return 0;

}



