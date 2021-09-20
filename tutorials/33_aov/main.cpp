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

	MatballScene::MATBALL matBall0 = matballScene.Init(context);

	rpr_material_node material = nullptr;
	CHECK(  rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_DIFFUSE, &material));
	CHECK(  rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_COLOR, 0.0f, 0.4f, 1.f, 0.f)); // Diffuse color

	matBall0.SetMaterial(material);


	// Define the list of AOVs we want to render
	//
	struct AOV_FB
	{
		AOV_FB(const std::string& name_, rpr_aov aov_)
		{
			framebuffer = nullptr;
			framebuffer_res = nullptr;
			name = name_;
			aov = aov_;
		}
		rpr_framebuffer framebuffer;
		rpr_framebuffer framebuffer_res;
		std::string name;
		rpr_aov aov;
	};
	std::vector<AOV_FB> AOVlist;
	AOVlist.push_back(AOV_FB("geom_normal" ,RPR_AOV_GEOMETRIC_NORMAL));
	AOVlist.push_back(AOV_FB("shad_normal" ,RPR_AOV_SHADING_NORMAL));
	AOVlist.push_back(AOV_FB("obj_id"      ,RPR_AOV_OBJECT_ID));
	AOVlist.push_back(AOV_FB("uv"          ,RPR_AOV_UV));

	// Create the list of AOVs
	//
	for(auto& i : AOVlist)
	{
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer) );
		CHECK( rprContextSetAOV(context, i.aov, i.framebuffer) );
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer_res) );
	}

	matballScene.Render("33_color.png");

	// for each AOV, export it ot an image file and delete it.
	//
	for(auto& i : AOVlist)
	{
		CHECK( rprContextResolveFrameBuffer(context, i.framebuffer, i.framebuffer_res, false) );
		CHECK( rprFrameBufferSaveToFile(i.framebuffer_res,  std::string("33_"+i.name+".png").c_str() ) );
		CHECK( rprObjectDelete(i.framebuffer) );
		CHECK( rprObjectDelete(i.framebuffer_res) );
	}
	AOVlist.clear();

	CHECK( rprObjectDelete(material)); material=nullptr;
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}





