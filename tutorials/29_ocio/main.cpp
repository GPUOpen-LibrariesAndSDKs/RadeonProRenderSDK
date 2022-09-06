/*****************************************************************************\
*
*  Module Name    OCIO Demo
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2022 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "../common/common.h"
#include <cassert>
#include <iostream>

#include "ocio_display.h"

//
//
// This demos shows how to use OCIO workflow.
// This workflow can be decomposed into 2 steps: 
// - Texture Color Space to Render Color Space (also called IDT)
// - Render Color Space to Display Color Space (also called RRT+ODT)
// 
// The IDT is managed internally by Radeon ProRender. You just need to configure the OCIO config file (with RPR_CONTEXT_OCIO_CONFIG_PATH) and the Color Space ( with RPR_CONTEXT_OCIO_RENDERING_COLOR_SPACE ).
// You also need to define the color space of each texture with rprImageSetOcioColorspace.
// A classic practice is to set :
// - "Utility - Linear - sRGB" for HDR textures
// - "Utility - Raw" for "data" textures, like normal maps, bump maps...
// - "Utility - sRGB - Texture" for LDR texture representing colors ( .png, .jpeg ... )
//
// The RRT+ODT is not managed by Radeon ProRender. A full demo of this step is done inside ocio_display.cpp.
// In order to execute this parts, you need to link OpenColorIO ( github.com/AcademySoftwareFoundation/OpenColorIO ) to this project, and set :
// #define USE_OCIO 1
// inside ocio_display.h
//
//


// simple garbage collector for RPR objects.
RPRGarbageCollector g_gc;


// Convert Color Space :
// From Linear sRGB to ACEScg
RadeonProRender::float4 sRGB_to_ACEScg(const RadeonProRender::float4& srgb)
{
	RadeonProRender::matrix sRGB__to__XYZ_D65(
		0.4124 , 0.3576, 0.1805,   0.0,
		0.2126,  0.7152, 0.0722,   0.0,
		0.0193,  0.1192,  0.9505,  0.0,
		0.0,      0.0,     0.0,    1.0);

	RadeonProRender::matrix D65_to_D60(
		1.01281,  0.00597,  -0.01497,  0.0,
		0.00749,  0.99834,   -0.00502,  0.0,
		-0.00285,   0.00471,   0.92424,  0.0,
		0.0,       0.0,        0.0,       1.0);

	RadeonProRender::matrix XYZd60_to_ACEScg(
		1.64184,  -0.32496,  -0.23654,  0.0,
		-0.66351,   1.61496,   0.01675,  0.0,
		0.01172,   -0.00828,   0.98866,  0.0,
		0.0,        0.0,        0.0,    1.0);

	RadeonProRender::matrix sRGB_to_ACEScg = XYZd60_to_ACEScg *  D65_to_D60 * sRGB__to__XYZ_D65 ;
	RadeonProRender::float4 ret = sRGB_to_ACEScg * srgb;
	return ret;
}



int main()
{
	//	for Debugging you can enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	rprContextSetParameterByKey1u(0,RPR_CONTEXT_TRACING_ENABLED,1);


	// If this variable is false, the Render Color Space will be sRGB.
	// If this variable is true, the Render Color Space will be ACEScg, and the ACEScg tonemapper will be used for RRT+ODT
	bool useOCIO = true;

	// if OCIO is not linked, then force this variable to FALSE.
	#if (USE_OCIO == 0)
	useOCIO = false;
	#endif


	// constant colors are set in the sRGB space.
	RadeonProRender::float4 color_leftWall   = RadeonProRender::float4(1.0, 0.0, 0.0);
	RadeonProRender::float4 color_rightWall = RadeonProRender::float4(0.025, 1.0, 0.05);
	RadeonProRender::float4 color_matball  = RadeonProRender::float4(0.2, 0.1, 0.9);
	RadeonProRender::float4 color_lampLeft = RadeonProRender::float4(30.0, 30.0, 10.0);
	RadeonProRender::float4 color_lampRight = RadeonProRender::float4(20.0, 20.0, 20.0);


	// for constant colors, the IDT needs to be managed manually.
	// If useOCIO is TRUE, then Render Color Space is ACEScg and we need to convert color constants to this color space.
	if ( useOCIO )
	{
		color_leftWall  = sRGB_to_ACEScg(color_leftWall);
		color_rightWall = sRGB_to_ACEScg(color_rightWall);
		color_matball   = sRGB_to_ACEScg(color_matball);
		color_lampLeft  = sRGB_to_ACEScg(color_lampLeft);
		color_lampRight = sRGB_to_ACEScg(color_lampRight);
	}


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


	const char ocio_config[] = "../../Resources/aces_1.0.3/config.ocio";

	if ( useOCIO )
	{
		// define the OCIO config file path
		CHECK( rprContextSetParameterByKeyString(context, RPR_CONTEXT_OCIO_CONFIG_PATH, ocio_config)  );

		// we are going to use the ACEScg for the Render Color Space.
		CHECK( rprContextSetParameterByKeyString(context, RPR_CONTEXT_OCIO_RENDERING_COLOR_SPACE, "ACES - ACEScg")  );
	}


	// Create the scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 0, 1, 10,    0, 2, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// Create an environment light
	{
		rpr_light lightEnv = nullptr;
		CHECK( rprContextCreateEnvironmentLight(context, &lightEnv));
		g_gc.GCAdd(lightEnv);

		const std::string pathImageFile = "../../Resources/Textures/turning_area_4k.hdr";

		rpr_image imgEnvLight = nullptr;
		rpr_status status = rprContextCreateImageFromFile(context, pathImageFile.c_str(), &imgEnvLight); // import image use by the Env light

		if (status == RPR_ERROR_IO_ERROR)
		{
			std::cout << "Error : " << pathImageFile << " not found.\n";
			return status;
		}
		CHECK(status);

		if ( useOCIO )
		{
			// for most of the cases, HDR textures are defined inside the "Utility - Linear - sRGB" color space.
			// ( meaning those textures are sRGB, without the gamma correction )
			CHECK( rprImageSetOcioColorspace(imgEnvLight , "Utility - Linear - sRGB") );
		}

		g_gc.GCAdd(imgEnvLight);

		// rotate the environment light.
		RadeonProRender::matrix matA =  RadeonProRender::rotation_y(-1.15f) ;
		rprLightSetTransform(lightEnv, true, &matA.m00);

		CHECK( rprEnvironmentLightSetImage(lightEnv, imgEnvLight));

		// set intensity
		CHECK( rprEnvironmentLightSetIntensityScale(lightEnv, 0.8f)); 

		CHECK( rprSceneAttachLight(scene, lightEnv));
	
	}

	

	// Add the matball shape.
	//
	rpr_shape matball0 =  ImportOBJ("../../Resources/Meshes/matball_base.obj",scene,context);
	g_gc.GCAdd(matball0);
	rpr_shape matball1 = ImportOBJ("../../Resources/Meshes/matball_inner.obj",scene,context);
	g_gc.GCAdd(matball1);
	rpr_shape matball2 = ImportOBJ("../../Resources/Meshes/matball_outer.obj",scene,context);
	g_gc.GCAdd(matball2);
	RadeonProRender::matrix mB = RadeonProRender::translation( RadeonProRender::float3(-2.0, 0.0 ,-1.0) ) * RadeonProRender::scale(RadeonProRender::float3(5.0,5.0,5.0)) * RadeonProRender::rotation_x(-MY_PI/2.0) ;
	CHECK( rprShapeSetTransform(matball0, true, &mB.m00));
	CHECK( rprShapeSetTransform(matball1, true, &mB.m00));
	CHECK( rprShapeSetTransform(matball2, true, &mB.m00));


	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	rpr_image imageInputA = NULL; 
	CHECK( rprContextCreateImageFromFile(context,"../../Resources/Textures/Glass_Antique_normal.jpg",&imageInputA));
	g_gc.GCAdd(imageInputA);

	if ( useOCIO )
	{
		// for textures that does not represent color ( like normal maps, bump maps... ), 
		// then "Utility - Raw" should be defined as color space.
		// this means OCIO will not do any conversion to this texture.
		CHECK( rprImageSetOcioColorspace(imageInputA , "Utility - Raw" ) );
	}

	// create the normal map material ( used by the matball shape )
	rpr_material_node textureNormalMap = NULL; 
	CHECK(   rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_NORMAL_MAP, &textureNormalMap));
	g_gc.GCAdd(textureNormalMap);
	rpr_material_node tex_A = NULL; 
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &tex_A));
	g_gc.GCAdd(tex_A);
	CHECK( rprMaterialNodeSetInputImageDataByKey(tex_A, RPR_MATERIAL_INPUT_DATA, imageInputA));
	CHECK( rprMaterialNodeSetInputNByKey(textureNormalMap, RPR_MATERIAL_INPUT_COLOR, tex_A));
	CHECK( rprMaterialNodeSetInputFByKey(textureNormalMap, RPR_MATERIAL_INPUT_SCALE, 1.0, 1.0, 1.0, 1.0));


	// create the material of the matball shape
	rpr_material_node material = nullptr;
	CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_UBERV2, &material));
	g_gc.GCAdd(material);
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR, color_matball.x, color_matball.y, color_matball.z, 0.f));
	CHECK( rprMaterialNodeSetInputNByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL, textureNormalMap  ));
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT, 1.f, 1.f, 1.f, 1.f));
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR, 0.5f, 0.5f, 0.5f, 1.f));
	CHECK( rprMaterialNodeSetInputNByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL, textureNormalMap  ));
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT, 0.9f, 0.9f, 0.9f, 1.f));
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS, 0.1f, 0.f, 0.f, 0.f));
	CHECK( rprMaterialNodeSetInputUByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE, RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
	CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_UBER_REFLECTION_METALNESS, 0.f, 0.f, 0.f, 1.f));

	// apply the material on the matball shape
	CHECK( rprShapeSetMaterial(matball0, material) );
	CHECK( rprShapeSetMaterial(matball1, material) );
	CHECK( rprShapeSetMaterial(matball2, material) );


	//
	// create the geometry of the scene. It's just a set of quads.
	//
	const float roofY = 3.0f;
	const float lightX0 = -4.0f + 0.4f;
	const float lightX1 = lightX0 + 0.8f;
	const float lightX2 = 4.0f - 0.4f - 0.8f;
	const float lightX3 = lightX2 + 0.8f;
	rpr_shape roof0 = CreateQuad_XZ(g_gc, context, scene,   -4.0f, -2.0f             ,    lightX0, +2.0f                    ,  roofY, -1.0f);
	rpr_shape roof1 = CreateQuad_XZ(g_gc, context, scene,   lightX0, -2.0f             ,  lightX1  , -2.0f+0.8*2.0f          ,  roofY, -1.0f);
	rpr_shape roof2 = CreateQuad_XZ(g_gc, context, scene,   lightX0, -2.0f+0.8*3.0f    ,  lightX1  , +2.0f                   ,  roofY, -1.0f);
	rpr_shape roofLightLeft = CreateQuad_XZ(g_gc, context, scene,   lightX0, -2.0f+0.8*2.0f    ,  lightX1  , -2.0f+0.8*3.0f                   ,  roofY, -1.0f);
	rpr_shape roof3 = CreateQuad_XZ(g_gc, context, scene,   lightX1, -2.0f             ,    lightX2, +2.0f                    ,  roofY, -1.0f);
	rpr_shape roof4 = CreateQuad_XZ(g_gc, context, scene,   lightX2, -2.0f             ,  lightX3  , -2.0f+0.8*2.0f          ,  roofY, -1.0f);
	rpr_shape roof5 = CreateQuad_XZ(g_gc, context, scene,   lightX2, -2.0f+0.8*3.0f    ,  lightX3  , +2.0f                   ,  roofY, -1.0f);
	rpr_shape roofLightRight = CreateQuad_XZ(g_gc, context, scene,   lightX2, -2.0f+0.8*2.0f    ,  lightX3  , -2.0f+0.8*3.0f                   ,  roofY, -1.0f);
	rpr_shape roof6 = CreateQuad_XZ(g_gc, context, scene,   lightX3, -2.0f             ,    4.0f, +2.0f                    ,  roofY, -1.0f);
	rpr_shape floor = CreateQuad_XZ(g_gc, context, scene,   -4.0f, -2.0f             ,    +4.0f, +2.0f                    ,  0.0f, +1.0f);
	rpr_shape shapeWallLeft = CreateQuad_YZ(g_gc, context, scene,   0.0, -2.0f             ,    roofY, +2.0f                    ,  -4.0f, 1.0f);
	rpr_shape shapeWallRight = CreateQuad_YZ(g_gc, context, scene,   0.0, -2.0f             ,    roofY, +2.0f                    ,  +4.0f, 1.0f);
	rpr_shape shapeWallBack = CreateQuad_XY(g_gc, context, scene,  -4.0, 0.0,     +4.0f, roofY        , -2.0f, 1.0f);
	rpr_shape pictureShape = CreateQuad_XY(g_gc, context, scene,   0.0, 0.3,     +3.5f, roofY-0.3f        , -1.9f, 1.0f);


	// create material for the left wall
	{
		rpr_material_node diffuse1=nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		g_gc.GCAdd(diffuse1);
		CHECK( rprMaterialNodeSetInputFByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, color_leftWall.x, color_leftWall.y, color_leftWall.z, 0.0));
		CHECK( rprShapeSetMaterial(shapeWallLeft, diffuse1));
	}

	// create material for the right wall
	{
		rpr_material_node diffuse1=nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		g_gc.GCAdd(diffuse1);
		CHECK( rprMaterialNodeSetInputFByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, color_rightWall.x, color_rightWall.y, color_rightWall.z, 0.0));
		CHECK( rprShapeSetMaterial(shapeWallRight, diffuse1));
	}


	// create material for the "art" picture
	{
		rpr_image img=nullptr;
		rpr_material_node imgSampler = nullptr;

		CHECK( rprContextCreateImageFromFile(context, "../../Resources/Textures/art.jpg",&img));
		g_gc.GCAdd(img);

		if ( useOCIO )
		{
			// For texture representing colors ( like Diffuse map ) ,
			// and if this texture is LDR ( .png, .jpeg ... ) ,
			// then in most of the cases, "Utility - sRGB - Texture" should be set as Color Space.
			// This means the texture is using the sRGB color space with a gamma correction.
			CHECK( rprImageSetOcioColorspace(img , "Utility - sRGB - Texture") );
		}
		else
		{
			// art.jpg image is stored in the non-linear sRGB space.
			// if we are not using the OCIO workflow, then we need to set the gamma for non-linear images ( like most of .png, .jpg... representing color )
			// This gamma value is used to do a basic IDT operation, transforming the image from non-linear "Texture Colorspace" to a linear "Render Colorspace".
			// In our case: from "non-linear sRGB" to "linear sRGB"
			CHECK( rprImageSetGamma(img, 2.2) );
		}

		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &imgSampler));
		g_gc.GCAdd(imgSampler);
		CHECK( rprMaterialNodeSetInputImageDataByKey(imgSampler, RPR_MATERIAL_INPUT_DATA, img)); // Set image data
		rpr_material_node diffuse1=nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse1));
		g_gc.GCAdd(diffuse1);
		CHECK( rprMaterialNodeSetInputNByKey(diffuse1, RPR_MATERIAL_INPUT_COLOR, imgSampler));
		CHECK( rprShapeSetMaterial(pictureShape, diffuse1));
	}


	// apply the emissive material on the left light
	{
		rpr_material_node material=nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_EMISSIVE, &material));
		g_gc.GCAdd(material);
		CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_COLOR, color_lampLeft.x, color_lampLeft.y, color_lampLeft.z, 0.0));
		CHECK( rprShapeSetMaterial(roofLightLeft, material));
	}

	// apply the emissive material on the right light
	{
		rpr_material_node material=nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_EMISSIVE, &material));
		g_gc.GCAdd(material);
		CHECK( rprMaterialNodeSetInputFByKey(material, RPR_MATERIAL_INPUT_COLOR, color_lampRight.x, color_lampRight.y, color_lampRight.z, 0.0));
		CHECK( rprShapeSetMaterial(roofLightRight, material));
	}


	// Create framebuffer 
	rpr_framebuffer_desc desc = { 1200 , 720 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	// Set rendering gamma.
	// When using OCIO, it's important to keep gamma to 1.0. This is because the output of the Render Color Space is supposed to be linear.
	// As OcioDisplay::Display is going to do the RRT+ODT, we need to give to this function a linear frame_buffer_resolved.
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 1.0f ) );

	// Render the scene
	CHECK( rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 200));
	CHECK( rprContextRender(context) );
	CHECK( rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));

	// Do RRT+ODT. This means we convert from the Linear Render Color Space, to the Non-Linear Display Color Space.
	// When converting from ACEScg to sRGB, a "S-Curve" is applied by OCIO for the tonemapping.
	//
	// the config.ocio file is defined like that:
	// ......
	//	displays:
	//  ACES:
	//	- !<View> {name: sRGB, colorspace: Output - sRGB}
	// ......
	//
	// We want the out colorspace : "Output - sRGB",  so we select display="ACES" and view="sRGB"
	//
	OcioDisplay::Display(
		frame_buffer_resolved, 
		ocio_config,
		"ACES - ACEScg",  
		"ACES", 
		"sRGB", 
		1.0f, // exposure
		2.2f , // display gamma
		"27_0.png",
		useOCIO
		);

	// Release the stuff we created
	CHECK( rprObjectDelete(camera)); camera=nullptr;
	CHECK( rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK( rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	g_gc.GCClean();
	CHECK( rprObjectDelete(scene)); scene=nullptr;
	CHECK( rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK( rprObjectDelete(context)); context=nullptr;
	return 0;
}




