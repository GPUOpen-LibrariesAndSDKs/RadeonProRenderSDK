/*****************************************************************************\
*
*  Module Name    MaterialX Demo
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
#include "RadeonProRender_MaterialX.h" // this header is needed for MaterialX

#include <cassert>
#include <iostream>
#include <memory>

//
// This demo illustrates how to use MaterialX with Radeon ProRender
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

	MatballScene::MATBALL matBall0 = matballScene.Init(context,-1,0);


	// Demo for the dielectric_bsdf MaterialX
	{ 
		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);
		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_dielectricBrdf.mtlx") );
		matBall0.SetMaterial(matx);

		matballScene.CameraLook1Shape(-1,0);
		matballScene.Render("26_materialx_00.png");
	}

	// Demo for the generalized_schlick_bsdf MaterialX
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,0);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);
		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_generalizedSchlickBrdf.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(0,0);
		matballScene.Render("26_materialx_01.png");
	}

	// Procedural MaterialX Demo
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(1,0);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);
		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_noise3d.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(1,0);
		matballScene.Render("26_materialx_02.png");
	}

	// Normal Lookup MaterialX Demo
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,-1);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);
		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_normal.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(-1,-1);
		matballScene.Render("26_materialx_03.png");
	}


	// Demo using the "Autodesk Standard Surface node definition" MaterialX.
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-1);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);

		// as matx_standard_surface_gold.mtlx is going to use the "standard_surface" material, we need to include the file before
		// calling rprMaterialXSetFile
		CHECK( rprMaterialXAddDependencyMtlx(context,"../../Resources/Textures/matx_standard_surface.mtlx") );

		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_standard_surface_gold.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(0,-1);
		matballScene.Render("26_materialx_04.png");

		// a simple clean of the include list defined by rprMaterialXAddDependencyMtlx.
		// We could choose to keep this dependency for the next renderings of this demo, but we choose to clean here 
		// in order to have more independent MaterialX demos.
		CHECK( rprMaterialXCleanDependencyMtlx(context) );
	}

	// Another demo using the "Autodesk Standard Surface node definition" MaterialX.
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(1,-1);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);

		// Note that we don't call rprMaterialXAddDependencyMtlx of "matx_standard_surface.mtlx" here.
		// This is because the standard material is already defined inside an  <xi:include  in matx_standard_surface_xi_carPaint.mtlx

		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_standard_surface_xi_carPaint.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(1,-1);
		matballScene.Render("26_materialx_05.png");
	}

	
	// Demo using the "USD Preview Surface" MaterialX.
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(-1,-2);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);

		// include of the "USD Preview Surface" definition
		CHECK( rprMaterialXAddDependencyMtlx(context,"../../Resources/Textures/matx_usd_preview_surface.mtlx") );

		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_usd_preview_surface_plastic.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(-1,-2);
		matballScene.Render("26_materialx_06.png");

		CHECK( rprMaterialXCleanDependencyMtlx(context) );
	}


	// This Demo shows that we also can render materials using image files.
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(0,-2);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);

		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_texture.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(0,-2);
		matballScene.Render("26_materialx_07.png");

		// Note that the release of images loaded by rprMaterialXSetFile is managed internally, during the rprObjectDelete(matx);
		// So API user doesn't need to take care if it.
	}

	// Another Demo using image files.
	// But this time, we choose to preload the image ourselves rather than letting rprMaterialXSetFile do it.
	// Advantage is that we have a better control of image parameters, and memory optimization if image is used in several MaterialX.
	// However we are responsible if its release.
	rpr_image preloadImg = nullptr;
	{
		MatballScene::MATBALL matBall = matballScene.AddMatball(1,-2);

		rpr_material_node matx = nullptr;
		CHECK( rprMaterialSystemCreateNode(matballScene.m_matsys, RPR_MATERIAL_NODE_MATX, &matx));
		g_gc.GCAdd(matx);

		// Create the rpr_image that will be used in the MaterialX rendering

		rpr_image_format imageFormat;
		memset(&imageFormat,0,sizeof(imageFormat));
		imageFormat.num_components = 4;
		imageFormat.type = RPR_COMPONENT_TYPE_UINT8;

		rpr_image_desc imageDesc;
		memset(&imageDesc,0,sizeof(imageDesc));
		imageDesc.image_depth = 1;
		imageDesc.image_width = 32;
		imageDesc.image_height = 32;
		imageDesc.image_row_pitch = imageDesc.image_width * sizeof(unsigned char)*4;
		imageDesc.image_slice_pitch = imageDesc.image_width * imageDesc.image_height * sizeof(unsigned char)*4;

		std::unique_ptr<char[]> dataImage(new char[imageDesc.image_width * imageDesc.image_height  *sizeof(unsigned char)*4]());

		// create a chessboard image.
		for(int i=0; i<imageDesc.image_width*imageDesc.image_height; i++)
		{
			int x = i%imageDesc.image_width;
			int y = (int)(i/imageDesc.image_width);
			if ( x % 16 < 8 && y % 16 < 8  ||   x % 16 >= 8 && y % 16 >= 8 )
			{
				dataImage[i*4+0] = 0;
				dataImage[i*4+1] = 128;
				dataImage[i*4+2] = 255;
			}
			else
			{
				dataImage[i*4+0] = 128;
				dataImage[i*4+1] = 51;
				dataImage[i*4+2] = 255;
			}
			dataImage[i*4+3] = 255;
		}

		// Most of RPR demos are using rprContextCreateImageFromFile.
		// Here is a demo of rprContextCreateImage, creating an image directly from data instead of image file.
		CHECK( rprContextCreateImage(context, imageFormat, &imageDesc, dataImage.get(), &preloadImg));

		// This rprMaterialXAddPreloadedImage specifies that any  "../../Resources/Textures/textest.png"  path used in the 
		// matx_standard_surface_gold_texture.mtlx file should use our preloaded 'preloadImg'.
		CHECK( rprMaterialXAddPreloadedImage(context, "../../Resources/Textures/textest.png", preloadImg) );

		CHECK( rprMaterialXSetFile(matx,"../../Resources/Textures/matx_standard_surface_metal_texture.mtlx") );
		matBall.SetMaterial(matx);

		matballScene.CameraLook1Shape(1,-2);
		matballScene.Render("26_materialx_08.png");

		// Clean the list created by any previous call of rprMaterialXAddPreloadedImage.
		// Note that this call does NOT delete image data: it just clear the lookup list.
		CHECK( rprMaterialXCleanPreloadedImages(context) );

	}



	// display the final scene : camera zoomed-out
	matballScene.CameraLook9Shape();
	matballScene.Render("26_materialx_final.png");
	
	
	//
	// clean everything
	//

	g_gc.GCClean();

	// delete the preloaded image.
	CHECK(rprObjectDelete(preloadImg)); preloadImg=nullptr;

	matballScene.Clean();
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;

	return 0;
}



