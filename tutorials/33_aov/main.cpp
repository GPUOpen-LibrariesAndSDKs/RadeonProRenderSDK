/*****************************************************************************\
*
*  Module Name    AOV demo
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
// This demo covers AOV (Arbitrary Output Variables), providing way to render different material component - mostly used for scene debugging.
// It also illustrates how to export special AOVs like Light Path Expression (LPE) and Cryptomatte.
//
//


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

	rpr_material_node material = nullptr;
	CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_DIFFUSE, &material));
	CHECK(  rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_COLOR, 0.0f, 0.4f, 1.f, 0.f)); // Diffuse color

	matBall0.SetMaterial(material);
	
	// Define the list of AOVs we want to render
	//
	struct AOV_FB
	{
		AOV_FB(const std::string& name_, rpr_aov aov_, bool applyDisplayGamma_)
		{
			framebuffer = nullptr;
			framebuffer_res = nullptr;
			name = name_;
			aov = aov_;
			applyDisplayGamma = applyDisplayGamma_; // should be set to TRUE for AOV representing colors (like RPR_AOV_COLOR)
		}
		rpr_framebuffer framebuffer;
		rpr_framebuffer framebuffer_res;
		std::string name;
		rpr_aov aov;
		bool applyDisplayGamma;
	};
	std::vector<AOV_FB> AOVlist;
	AOVlist.push_back(AOV_FB("geom_normal" ,RPR_AOV_GEOMETRIC_NORMAL, false));
	AOVlist.push_back(AOV_FB("shad_normal" ,RPR_AOV_SHADING_NORMAL, false));
	AOVlist.push_back(AOV_FB("obj_id"      ,RPR_AOV_OBJECT_ID, false));
	AOVlist.push_back(AOV_FB("uv"          ,RPR_AOV_UV, false));
	AOVlist.push_back(AOV_FB("lpe0"        ,RPR_AOV_LPE_0, true)); // bind this framebuffer to the first LPE (Light Path Expression) channel
	AOVlist.push_back(AOV_FB("lpe1"        ,RPR_AOV_LPE_1, true)); // bind this framebuffer to the second LPE channel
	AOVlist.push_back(AOV_FB("cam_normal"  ,RPR_AOV_CAMERA_NORMAL, false));
	
	// Create the list of AOVs
	//
	for(auto& i : AOVlist)
	{
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer) );
		CHECK( rprContextSetAOV(context, i.aov, i.framebuffer) );
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer_res) );
	}

	// for LPE, we need to set the string defining the AOV behaviour.
	CHECK( rprFrameBufferSetLPE(AOVlist[4].framebuffer, "C<RD>L" )); // first bounce
	CHECK( rprFrameBufferSetLPE(AOVlist[5].framebuffer, "C<RD>[RT].*" )); // all but emissive-reflect-refract

	matballScene.Render("33_color.png");

	// for each AOV, export it to an image file and delete it.
	//
	for(auto& i : AOVlist)
	{
		CHECK( rprContextResolveFrameBuffer(context, i.framebuffer, i.framebuffer_res, !i.applyDisplayGamma) );
		CHECK( rprFrameBufferSaveToFile(i.framebuffer_res,  std::string("33_"+i.name+".png").c_str() ) );
		CHECK( rprContextSetAOV(context, i.aov, nullptr) ); // detach AOV
		CHECK( rprObjectDelete(i.framebuffer) );
		CHECK( rprObjectDelete(i.framebuffer_res) );
	}
	AOVlist.clear();


	// Demo for Cryptomatte
	{
		// when generating Cryptomatte, it's important to name all shapes and materials.
		// Cryptomatte relies on Object Name and Material Name.
		CHECK( rprObjectSetName(matBall0.base,"base"));
		CHECK( rprObjectSetName(matBall0.inner,"inner"));
		CHECK( rprObjectSetName(matBall0.outer,"outer"));
		CHECK( rprObjectSetName(matballScene.m_shape_floor,"floor"));
		CHECK( rprObjectSetName(material,"matball"));
		CHECK( rprObjectSetName(matballScene.m_floorMaterial,"matfloor"));

		// define the list of AOV.
		// giving those exact string names for cryptomatte is mandatory as they are used internally to identify them.
		AOVlist.clear();
		AOVlist.push_back(AOV_FB("CryptoMaterial00" ,RPR_AOV_CRYPTOMATTE_MAT0, false));
		AOVlist.push_back(AOV_FB("CryptoMaterial01" ,RPR_AOV_CRYPTOMATTE_MAT1, false));
		AOVlist.push_back(AOV_FB("CryptoMaterial02" ,RPR_AOV_CRYPTOMATTE_MAT2, false));
		AOVlist.push_back(AOV_FB("CryptoObject00" ,RPR_AOV_CRYPTOMATTE_OBJ0, false));
		AOVlist.push_back(AOV_FB("CryptoObject01" ,RPR_AOV_CRYPTOMATTE_OBJ1, false));
		AOVlist.push_back(AOV_FB("CryptoObject02" ,RPR_AOV_CRYPTOMATTE_OBJ2, false));

		// create each framebuffer / name it / and attach it to AOV.
		// ( the COLOR AOV is already managed by matballScene )
		for(auto& i : AOVlist)
		{
			CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer) );
			CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer_res) );
			CHECK( rprObjectSetName(i.framebuffer_res, i.name.c_str()) ); // framebuffer names are important for cryptomattes.
			CHECK( rprContextSetAOV(context, i.aov, i.framebuffer) );
		}
		
		// render again with the new AOV configuration
		CHECK( rprFrameBufferClear(matballScene.m_frame_buffer) ); // clear the previous COLOR AOV framebuffer.
		std::cout << "Rendering 33_cryptomatte.exr" << std::endl;
		CHECK( rprContextRender(matballScene.m_context) );

		std::vector<rpr_framebuffer> fbResolved; // list of framebuffer exported inside rprFrameBufferSaveToFileEx

		// add the color framebuffer.
		fbResolved.push_back(matballScene.m_frame_buffer_res);
		CHECK( rprObjectSetName(matballScene.m_frame_buffer_res, "color") ); // <- this name will be used inside the EXR file.

		// add each resolved cryptomatte AOV.
		for(auto& i : AOVlist)
		{
			CHECK( rprContextResolveFrameBuffer(context, i.framebuffer, i.framebuffer_res, !i.applyDisplayGamma) );
			fbResolved.push_back(i.framebuffer_res);
		}

		// rprFrameBufferSaveToFileEx is like rprFrameBufferSaveToFile except that it can store multilayers. Should only be used with EXR export.
		// Using multilayers is the only way to export cryptomattes correctly in Radeon ProRender.
		CHECK( rprFrameBufferSaveToFileEx( fbResolved.data(), (rpr_int)fbResolved.size(), "33_cryptomatte.exr", nullptr ) );

		// delete framebuffers
		for(auto& i : AOVlist)
		{
			CHECK( rprContextSetAOV(context, i.aov, nullptr) ); // detach AOV
			CHECK( rprObjectDelete(i.framebuffer) );
			CHECK( rprObjectDelete(i.framebuffer_res) );
		}
		AOVlist.clear();

	}


	CHECK( rprObjectDelete(material)); material=nullptr;
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}





