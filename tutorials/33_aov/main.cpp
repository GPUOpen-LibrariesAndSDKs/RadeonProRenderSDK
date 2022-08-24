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
		AOV_FB(const std::string& name_, rpr_aov aov_, bool useDisplayGamma_)
		{
			framebuffer = nullptr;
			framebuffer_res = nullptr;
			name = name_;
			aov = aov_;
			useDisplayGamma = useDisplayGamma_; 
		}
		rpr_framebuffer framebuffer;
		rpr_framebuffer framebuffer_res;
		std::string name;
		rpr_aov aov;
		bool useDisplayGamma; // Set this parameter to TRUE for AOV representing Colors. For other AOVs storing data ( Normals, Depth ... ) make sure to set it to FALSE.
	};
	std::vector<AOV_FB> AOVlist;
	AOVlist.push_back(AOV_FB("geom_normal" ,RPR_AOV_GEOMETRIC_NORMAL, false));
	AOVlist.push_back(AOV_FB("shad_normal" ,RPR_AOV_SHADING_NORMAL, false));
	AOVlist.push_back(AOV_FB("obj_id"      ,RPR_AOV_OBJECT_ID, false));
	AOVlist.push_back(AOV_FB("mat_id"      ,RPR_AOV_MATERIAL_ID, false));
	AOVlist.push_back(AOV_FB("uv"          ,RPR_AOV_UV, false));

	// Create the list of AOVs
	//
	for(auto& i : AOVlist)
	{
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer) );
		CHECK( rprContextSetAOV(context, i.aov, i.framebuffer) );
		CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &i.framebuffer_res) );
	}

	matballScene.Render("33_color.png");

	// for each AOV, export it ot an image file
	for(auto& i : AOVlist)
	{
		CHECK( rprContextResolveFrameBuffer(context, i.framebuffer, i.framebuffer_res, !i.useDisplayGamma) );
		CHECK( rprFrameBufferSaveToFile(i.framebuffer_res,  std::string("33_"+i.name+".png").c_str() ) );
	}


	// This is a demo for customized AOV lookup color :
	// For AOV representing ID ( like RPR_AOV_OBJECT_ID, RPR_AOV_MATERIAL_ID ), we can set custom colors for each ID instead of using the random colors assigned by the renderer.
	{
		std::cout<<"Demo for AOV lookup color..."<<std::endl;

		// assign red to ID=0, green to ID=1 ... etc...
		CHECK( rprContextSetAOVindexLookup(context, 0,    1.0, 0.0, 0.0, 0.0) ); // red
		CHECK( rprContextSetAOVindexLookup(context, 1,    0.0, 1.0, 0.0, 0.0) ); // green
		CHECK( rprContextSetAOVindexLookup(context, 2,    0.0, 0.0, 1.0, 0.0) ); // blue
		CHECK( rprContextSetAOVindexLookup(context, 3,    1.0, 1.0, 0.0, 0.0) ); // yellow
		CHECK( rprContextSetAOVindexLookup(context, 4,    0.0, 1.0, 1.0, 0.0) ); // cyan
		CHECK( rprContextSetAOVindexLookup(context, 99,   1.0, 0.0, 1.0, 0.0) ); // magenta

		// demo for the rprShapeSetObjectID function.
		// in this rendering, the inner ball will be rendered as mangenta inside the RPR_AOV_OBJECT_ID AOV. this is because the index 99 has been assigned to the magenta color
		CHECK( rprShapeSetObjectID(matBall0.inner, 99) );
		
		// clear the framebuffers before starting a new rendering
		for(auto& i : AOVlist)
		{
			CHECK( rprFrameBufferClear(i.framebuffer) );
		}
		CHECK( rprFrameBufferClear(matballScene.m_frame_buffer) );

		// render the scene
		CHECK( rprContextRender(matballScene.m_context) );

		// resolve RPR_AOV_OBJECT_ID and RPR_AOV_MATERIAL_ID AOV, and export them to PNG.
		CHECK( rprContextResolveFrameBuffer(context, AOVlist[2].framebuffer, AOVlist[2].framebuffer_res, true) );
		CHECK( rprContextResolveFrameBuffer(context, AOVlist[3].framebuffer, AOVlist[3].framebuffer_res, true) );
		CHECK( rprFrameBufferSaveToFile(AOVlist[2].framebuffer_res,  std::string("33_obj_id_2.png").c_str() ) );
		CHECK( rprFrameBufferSaveToFile(AOVlist[3].framebuffer_res,  std::string("33_mat_id_2.png").c_str() ) );

		// You will notice that colors from  33_obj_id_2.png  are different compared to 33_obj_id.png.
		// Same thing for 33_mat_id.
	}


	// This is a demo for integer framebuffer
	// For AOV representing ID ( like RPR_AOV_OBJECT_ID, RPR_AOV_MATERIAL_ID ), we can choose to export them as 32bits integer format framebuffer instead of 3-float colors.
	{
		std::cout<<"Demo for integer framebuffer..."<<std::endl;

		// create an Integer framebuffer :
		rpr_framebuffer framebuffer_INT_objID = nullptr;
		rpr_framebuffer framebuffer_INT_objID_resolve = nullptr;
		rpr_framebuffer_format fmt_INT = {4, RPR_COMPONENT_TYPE_UINT32}; // note that we use the UINT32 format, instead of FLOAT32
		CHECK(  rprContextCreateFrameBuffer(context, fmt_INT, &matballScene.m_framebuffer_desc, &framebuffer_INT_objID) );
		CHECK(  rprContextCreateFrameBuffer(context, fmt_INT, &matballScene.m_framebuffer_desc, &framebuffer_INT_objID_resolve) );

		// assign this AOV to store Object ID
		CHECK(  rprContextSetAOV(context, RPR_AOV_OBJECT_ID, framebuffer_INT_objID) );

		// set the floor shape ID to 0x12345678
		CHECK( rprShapeSetObjectID(matballScene.m_shape_floor, 0x12345678) );

		// clear color framebuffer before rendering ( because it's was used previously )
		CHECK( rprFrameBufferClear(matballScene.m_frame_buffer) );

		// render the scene
		CHECK( rprContextRender(matballScene.m_context) );

		// resolve the Object ID framebuffer
		CHECK( rprContextResolveFrameBuffer(context, framebuffer_INT_objID, framebuffer_INT_objID_resolve, true) );

		// read data from the resolved Object ID framebuffer.
		size_t frame_buffer_resolve_dataSize = 0;
		CHECK(  rprFrameBufferGetInfo(framebuffer_INT_objID_resolve, RPR_FRAMEBUFFER_DATA, 0 , NULL , &frame_buffer_resolve_dataSize ) );
		uint32_t* frame_buffer_resolve_data = (uint32_t*)malloc(frame_buffer_resolve_dataSize);
		CHECK(  rprFrameBufferGetInfo(framebuffer_INT_objID_resolve, RPR_FRAMEBUFFER_DATA, frame_buffer_resolve_dataSize , frame_buffer_resolve_data , NULL ) );

		// data of the first pixel:
		frame_buffer_resolve_data[0]; // <-- this value will be 0x12345678 because this pixel represents the floor shape
		frame_buffer_resolve_data[1];
		frame_buffer_resolve_data[2];
		frame_buffer_resolve_data[3];

		// data of the second pixel:
		frame_buffer_resolve_data[4]; // <-- same value than frame_buffer_resolve_data[0]
		frame_buffer_resolve_data[5];
		frame_buffer_resolve_data[6];
		frame_buffer_resolve_data[7];

		printf("frame_buffer_resolve_data[0] = 0x%x\n" , frame_buffer_resolve_data[0]);

		// clear stuff from this demo.
		free(frame_buffer_resolve_data); frame_buffer_resolve_data=nullptr;
		CHECK( rprObjectDelete(framebuffer_INT_objID) );
		CHECK( rprObjectDelete(framebuffer_INT_objID_resolve) );
	}


	// delete AOVs
	for(auto& i : AOVlist)
	{
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





