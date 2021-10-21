/*****************************************************************************\
*
*  Module Name    Contour demo
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
// Demo of the Contour Rendering (also called Outline) that allows you to render contours of 3D objects.
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

	// create the matball scene.
	MatballScene matballScene;
	MatballScene::MATBALL matBall0 = matballScene.Init(context,0,0);


	//
	// For any Contour Rendering, we always need 4 AOV :   RPR_AOV_MATERIAL_ID, RPR_AOV_SHADING_NORMAL, RPR_AOV_OBJECT_ID, RPR_AOV_UV
	//
	rpr_framebuffer fb_matid = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &fb_matid) );
	CHECK( rprContextSetAOV(context, RPR_AOV_MATERIAL_ID, fb_matid) );

	rpr_framebuffer fb_shadNormal = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &fb_shadNormal) );
	CHECK( rprContextSetAOV(context, RPR_AOV_SHADING_NORMAL, fb_shadNormal) );

	rpr_framebuffer fb_objId = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &fb_objId) );
	CHECK( rprContextSetAOV(context, RPR_AOV_OBJECT_ID, fb_objId) );

	rpr_framebuffer fb_uv = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, matballScene.m_framebuffer_fmt, &matballScene.m_framebuffer_desc, &fb_uv) );
	CHECK( rprContextSetAOV(context, RPR_AOV_UV, fb_uv) );

	// Enable the Contour Rendering
	CHECK( rprContextSetParameterByKeyString(context, RPR_CONTEXT_GPUINTEGRATOR, "gpucontour") );
	

	//
	// Set the contour parameters
	//

	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_CONTOUR_USE_OBJECTID, 1) );
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_CONTOUR_USE_MATERIALID, 1) );
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_CONTOUR_USE_NORMAL, 1) );
	CHECK( rprContextSetParameterByKey1u(context, RPR_CONTEXT_CONTOUR_USE_UV, 0) ); // we don't want to use UV in this contour rendering

	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_NORMAL_THRESHOLD, 20.0) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_UV_THRESHOLD, 0.03) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_LINEWIDTH_OBJECTID, 3) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_LINEWIDTH_MATERIALID, 1) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_LINEWIDTH_NORMAL, 2.0f) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_LINEWIDTH_UV, 1.0f) );
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_CONTOUR_ANTIALIASING, 1) );


	// render the scene
	matballScene.Render("24_contour.png");


	// Delete RPR nodes.
	CHECK( rprObjectDelete(fb_matid) ); fb_matid=nullptr;
	CHECK( rprObjectDelete(fb_shadNormal) ); fb_shadNormal=nullptr;
	CHECK( rprObjectDelete(fb_objId) ); fb_objId=nullptr;
	CHECK( rprObjectDelete(fb_uv) ); fb_uv=nullptr;
	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;

}





