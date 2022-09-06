/*****************************************************************************\
*
*  Module Name    Advanced Texturing Demo
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

//
// This demo shows different features related to texture manipulation: Manage the UV, create procedural texture, use arthmetics and custom materials, texture wrapping.
//

// garbage collector of this demo
RPRGarbageCollector g_gc;

int g_x = 0;
int g_y = 0;

// create a simple square shape facing the camera
rpr_shape AddSquare(rpr_context context, rpr_scene scene)
{
	const float margin = 0.1f;
	const float size = 2.0f;
	rpr_shape quad = CreateQuad_XY(g_gc, context, scene,  
		-5.0f+ g_x*(margin+size)       , 1.1f+g_y*(margin+size)+size	,    
		-5.0f+ g_x*(margin+size)+size  , 1.1f+g_y*(margin+size)	,  
		0.0,
		1.0f);

	g_y --;
	if ( g_y == -3 )
	{
		g_x ++;
		g_y = 0;
	}

	return quad;
}


// this is good practice to declare a custom material.
// This makes sure that the GPU and CPU devices are executing the same source code.
// RprCustomMatA is combining 2 effects: swapping red and blue channel, and reducing luminosity on the borders of the image.
#define DEFINE_FUNC(FUNCNAME, FUNC) FUNC; const std::string g_str_##FUNCNAME = #FUNC;
DEFINE_FUNC( RprCustomMatA ,          void RprCustomMatA(float* a, float* b, float* c, float* d, float* e, float* f, float* valueOut){   valueOut[0]=c[1]*sin(d[0]*3.14159);  valueOut[1]=c[0]*sin(d[0]*3.14159);  valueOut[2]=c[2]*sin(d[0]*3.14159);       }      );




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

	// Create context
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set the active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "RPR Context creation succeeded." << std::endl;

	// Create the scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); 
	CHECK( rprContextSetScene(context, scene) );

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	g_gc.GCAdd(camera);
	CHECK( rprCameraLookAt(camera, 0, 0, 10.0f,    0, 0, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	
	// Create an environment light
	// For this demo we are going to use a simple 1x1 pixel white env light.
	// the image will be created with rprContextCreateImage ( instead of creating it from an image file )
	{
		rpr_light lightEnv = nullptr;
		CHECK(rprContextCreateEnvironmentLight(context, &lightEnv));
		g_gc.GCAdd(lightEnv);
	
		rpr_image_format imageFormat;
		memset(&imageFormat,0,sizeof(imageFormat));
		imageFormat.num_components = 4;
		imageFormat.type = RPR_COMPONENT_TYPE_FLOAT32;

		rpr_image_desc imageDesc;
		memset(&imageDesc,0,sizeof(imageDesc));
		imageDesc.image_depth = 1;
		imageDesc.image_width = 1;
		imageDesc.image_height = 1;
		imageDesc.image_row_pitch = imageDesc.image_width * sizeof(unsigned char)*4;
		imageDesc.image_slice_pitch = imageDesc.image_width * imageDesc.image_height * sizeof(unsigned char)*4;

		// white pixel
		float dataImage[4];
		dataImage[0] = 1.0;
		dataImage[1] = 1.0;
		dataImage[2] = 1.0;
		dataImage[3] = 1.0;

		rpr_image imgEnvLight = nullptr;
		CHECK( rprContextCreateImage(context, imageFormat, &imageDesc, dataImage, &imgEnvLight));
		g_gc.GCAdd(imgEnvLight);

		CHECK(rprEnvironmentLightSetImage(lightEnv, imgEnvLight));
		CHECK(rprEnvironmentLightSetIntensityScale(lightEnv, 1.0f)); 
		CHECK(rprSceneAttachLight(scene, lightEnv));
	}

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );

	// list of quads mesh facing the camera
	std::vector<rpr_shape> quads;

	// this flower texture will be used in several materials.
	rpr_image image1 = nullptr;
	const std::string pathImageFileA = "../../Resources/Textures/flower.jpg";
	CHECK( rprContextCreateImageFromFile(context, pathImageFileA.c_str(), &image1) );
	g_gc.GCAdd(image1);

	// texture sampler of the flower.jpg image
	rpr_material_node materialImage2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialImage2));
	g_gc.GCAdd(materialImage2);
	CHECK(rprMaterialNodeSetInputImageDataByKey(materialImage2, RPR_MATERIAL_INPUT_DATA, image1)); // Set image data

	// This texture is using the classic 2.2 gamma compression.
	CHECK( rprImageSetGamma(image1, 2.2f) );

	//
	// Simple image on the shape, without any additional process.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node diffuseMaterial = nullptr;
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMaterial));
		g_gc.GCAdd(diffuseMaterial);
		CHECK(rprMaterialNodeSetInputNByKey(diffuseMaterial, RPR_MATERIAL_INPUT_COLOR, materialImage2));

		// assign the material on the shape
		CHECK(rprShapeSetMaterial(quads.back(), diffuseMaterial));	
	}

	//
	// This demo shows how to manipulate the UV: here we are rotating and scaling the image on the shape.
	//
	{
		quads.push_back( AddSquare(context, scene) );


		rpr_material_node materialNodeTexture = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialNodeTexture) );
		g_gc.GCAdd(materialNodeTexture);
		CHECK( rprMaterialNodeSetInputImageDataByKey(materialNodeTexture, RPR_MATERIAL_INPUT_DATA, image1) );

		// scaling value of the UV
		float scaleUVx = 2.5f;
		float scaleUVy = 2.5f;

		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		rpr_material_node uvScaled = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uvScaled) );
		g_gc.GCAdd(uvScaled);
		CHECK( rprMaterialNodeSetInputUByKey(uvScaled, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL) );
		CHECK( rprMaterialNodeSetInputNByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR0, uv_node) );
		CHECK( rprMaterialNodeSetInputFByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR1, scaleUVx, scaleUVy, 0.0f, 0) );

		// roation angle of the UV
		float teta = MY_PI / 6.0f;

		rpr_material_node arith1 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &arith1) );
		g_gc.GCAdd(arith1);
		CHECK( rprMaterialNodeSetInputUByKey(arith1, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_DOT3) );
		CHECK( rprMaterialNodeSetInputNByKey(arith1, RPR_MATERIAL_INPUT_COLOR0, uvScaled) );
		CHECK( rprMaterialNodeSetInputFByKey(arith1, RPR_MATERIAL_INPUT_COLOR1, cosf(teta), sinf(teta), 0, 0) );

		rpr_material_node arith2 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &arith2) );
		g_gc.GCAdd(arith2);
		CHECK( rprMaterialNodeSetInputUByKey(arith2, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_DOT3) );
		CHECK( rprMaterialNodeSetInputNByKey(arith2, RPR_MATERIAL_INPUT_COLOR0, uvScaled) );
		CHECK( rprMaterialNodeSetInputFByKey(arith2, RPR_MATERIAL_INPUT_COLOR1, -sinf(teta) , cosf(teta),  0, 0) );

		// the output of RPR_MATERIAL_NODE_OP_COMBINE is : float4(col0.x, col1.x, col0.y, col1.y)
		rpr_material_node combine = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &combine) );
		g_gc.GCAdd(combine);
		CHECK( rprMaterialNodeSetInputUByKey(combine, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_COMBINE) );
		CHECK( rprMaterialNodeSetInputNByKey(combine, RPR_MATERIAL_INPUT_COLOR0, arith1) );
		CHECK( rprMaterialNodeSetInputNByKey(combine, RPR_MATERIAL_INPUT_COLOR1, arith2) );

		CHECK( rprMaterialNodeSetInputNByKey(materialNodeTexture, RPR_MATERIAL_INPUT_UV, combine ) );

		rpr_material_node diffuseSphere = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseSphere));
		g_gc.GCAdd(diffuseSphere);
		CHECK( rprMaterialNodeSetInputNByKey(diffuseSphere, RPR_MATERIAL_INPUT_COLOR, materialNodeTexture ));

		CHECK(rprShapeSetMaterial(quads.back(), diffuseSphere));
		
	}

	//
	// demo of the Voronoi procedural texture.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node voronoi = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_VORONOI_TEXTURE, &voronoi));
		g_gc.GCAdd(voronoi);

		rpr_material_node diffuse = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse));
		g_gc.GCAdd(diffuse);
		CHECK( rprMaterialNodeSetInputNByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, voronoi ));

		CHECK( rprMaterialNodeSetInputUByKey(voronoi, RPR_MATERIAL_INPUT_OUTTYPE, RPR_VORONOI_OUT_TYPE_COLOR  ));

		CHECK(rprShapeSetMaterial(quads.back(), diffuse));
	}

	//
	// demo of the Ramp material.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		rpr_material_node plane_shader = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_RAMP, &plane_shader) );
		g_gc.GCAdd(plane_shader);

		CHECK( rprMaterialNodeSetInputNByKey(plane_shader, RPR_MATERIAL_INPUT_UV, uv_node) );

		rpr_material_node diffuseFinalTest = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseFinalTest) );
		g_gc.GCAdd(diffuseFinalTest);
		CHECK( rprMaterialNodeSetInputNByKey(diffuseFinalTest, RPR_MATERIAL_INPUT_COLOR, plane_shader) );

		const int nControlPoints = 5;
		RadeonProRender::float4 controlPoints[nControlPoints] = {
			RadeonProRender::float4(0.0f,  1.f, 0.f, 0.f),
			RadeonProRender::float4(0.2f,  0.f, 1.f, 0.f),
			RadeonProRender::float4(0.5f,  0.f, 0.f, 1.f),
			RadeonProRender::float4(0.6f,  1.f, 1.f, 0.f),
			RadeonProRender::float4(1.0f,  1.f, 0.f, 1.f)
			};
		int bufferSize = nControlPoints*sizeof(RadeonProRender::float4);
		CHECK( rprMaterialNodeSetInputDataByKey(plane_shader, RPR_MATERIAL_INPUT_DATA, controlPoints, bufferSize ));
		
		CHECK( rprMaterialNodeSetInputUByKey(plane_shader, RPR_MATERIAL_INPUT_TYPE, RPR_INTERPOLATION_MODE_LINEAR));

		CHECK(rprShapeSetMaterial(quads.back(), diffuseFinalTest));
	}

	//
	// Demo of the Noise2D procedural texture.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node noise2d = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_NOISE2D_TEXTURE, &noise2d));
		g_gc.GCAdd(noise2d);

		
		// In order to have more content demo of arithmetics, let's give this noise inside a sigmoid function :  1/(1+e^(-30*(x-0.5)))
		// the sigmoid is going to increase the contrast between the green and red in this demo.

		const float e = 2.718281828459045f;

		rpr_material_node op1 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op1) );
		g_gc.GCAdd(op1);
		CHECK( rprMaterialNodeSetInputUByKey(op1, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_SUB) );
		CHECK( rprMaterialNodeSetInputNByKey(op1, RPR_MATERIAL_INPUT_COLOR0, noise2d) );
		CHECK( rprMaterialNodeSetInputFByKey(op1, RPR_MATERIAL_INPUT_COLOR1, 0.5, 0.5, 0.5, 0.0) );

		rpr_material_node op2 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op2) );
		g_gc.GCAdd(op2);
		CHECK( rprMaterialNodeSetInputUByKey(op2, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL) );
		CHECK( rprMaterialNodeSetInputNByKey(op2, RPR_MATERIAL_INPUT_COLOR0, op1) );
		CHECK( rprMaterialNodeSetInputFByKey(op2, RPR_MATERIAL_INPUT_COLOR1, -30.0f, -30.0f, -30.0f, 0.0) );

		rpr_material_node op3 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op3) );
		g_gc.GCAdd(op3);
		CHECK( rprMaterialNodeSetInputUByKey(op3, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_POW) );
		CHECK( rprMaterialNodeSetInputFByKey(op3, RPR_MATERIAL_INPUT_COLOR0, e,e,e, 0.0) );
		CHECK( rprMaterialNodeSetInputNByKey(op3, RPR_MATERIAL_INPUT_COLOR1, op2) );

		rpr_material_node op4 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op4) );
		g_gc.GCAdd(op4);
		CHECK( rprMaterialNodeSetInputUByKey(op4, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_ADD) );
		CHECK( rprMaterialNodeSetInputFByKey(op4, RPR_MATERIAL_INPUT_COLOR0, 1.0,1.0,1.0, 0.0) );
		CHECK( rprMaterialNodeSetInputNByKey(op4, RPR_MATERIAL_INPUT_COLOR1, op3) );

		rpr_material_node op5 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op5) );
		g_gc.GCAdd(op5);
		CHECK( rprMaterialNodeSetInputUByKey(op5, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_DIV) );
		CHECK( rprMaterialNodeSetInputFByKey(op5, RPR_MATERIAL_INPUT_COLOR0, 1.0,1.0,1.0, 0.0) );
		CHECK( rprMaterialNodeSetInputNByKey(op5, RPR_MATERIAL_INPUT_COLOR1, op4) );

		// create 2 colors for the final rendering of the noise.
		rpr_material_node d0 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d0));
		g_gc.GCAdd(d0);
		CHECK( rprMaterialNodeSetInputFByKey(d0, RPR_MATERIAL_INPUT_COLOR, 1,0,0,0 ));
		rpr_material_node d1 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d1));
		g_gc.GCAdd(d1);
		CHECK( rprMaterialNodeSetInputFByKey(d1, RPR_MATERIAL_INPUT_COLOR, 0,1,0,0 ));

		rpr_material_node blend = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_BLEND, &blend));
		g_gc.GCAdd(blend);
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR0, d0));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR1, d1));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_WEIGHT, op5)); // the noise is used as weight to mix the 2 colors.

		CHECK(rprShapeSetMaterial(quads.back(), blend));
	}

	//
	// Demo of the procedural Dot Texture
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node dotmat = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_DOT_TEXTURE, &dotmat));
		g_gc.GCAdd(dotmat);

		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		// scale the UV
		rpr_material_node uv_scaled_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node) );
		g_gc.GCAdd(uv_scaled_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL) );
		CHECK( rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node) );
		CHECK( rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 2.0, 1.0, 1.0, 0) );

		CHECK( rprMaterialNodeSetInputNByKey(dotmat, RPR_MATERIAL_INPUT_UV, uv_scaled_node));

		// create 2 colors for the final rendering of the dot.
		rpr_material_node d0 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d0));
		g_gc.GCAdd(d0);
		CHECK( rprMaterialNodeSetInputFByKey(d0, RPR_MATERIAL_INPUT_COLOR, 0,0,1,0 ));
		rpr_material_node d1 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d1));
		g_gc.GCAdd(d1);
		CHECK( rprMaterialNodeSetInputFByKey(d1, RPR_MATERIAL_INPUT_COLOR, 1,1,0,0 ));

		rpr_material_node blend = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_BLEND, &blend));
		g_gc.GCAdd(blend);
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR0, d0));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR1, d1));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_WEIGHT, dotmat)); // the dot texture is used as weight to mix the 2 colors.

		CHECK(rprShapeSetMaterial(quads.back(), blend));
	}

	//
	// demo of the RPR_MATERIAL_NODE_RGB_TO_HSV material.
	// RPR also supports RPR_MATERIAL_NODE_HSV_TO_RGB.
	//	
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node diffuseMaterial = nullptr;
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMaterial));
		g_gc.GCAdd(diffuseMaterial);
		
		rpr_material_node rgbHsv = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_RGB_TO_HSV, &rgbHsv));
		g_gc.GCAdd(rgbHsv);
		CHECK( rprMaterialNodeSetInputNByKey(rgbHsv, RPR_MATERIAL_INPUT_COLOR, materialImage2 ));

		CHECK(rprMaterialNodeSetInputNByKey(diffuseMaterial, RPR_MATERIAL_INPUT_COLOR, rgbHsv));

		CHECK(rprShapeSetMaterial(quads.back(), diffuseMaterial));
	}

	//
	// Demo of the procedural checker texture.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node dotmat = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_CHECKER_TEXTURE, &dotmat));
		g_gc.GCAdd(dotmat);

		// do some UV scaling of the checker :
		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		rpr_material_node uv_scaled_node;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uv_scaled_node) );
		g_gc.GCAdd(uv_scaled_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_scaled_node, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL) );
		CHECK( rprMaterialNodeSetInputNByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR0, uv_node) );
		CHECK( rprMaterialNodeSetInputFByKey(uv_scaled_node, RPR_MATERIAL_INPUT_COLOR1, 2.0, 2.0, 1.0, 0) );

		CHECK( rprMaterialNodeSetInputNByKey(dotmat, RPR_MATERIAL_INPUT_UV, uv_scaled_node));

		// create 2 colors for the final rendering of the checker.
		rpr_material_node d0 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d0));
		g_gc.GCAdd(d0);
		CHECK( rprMaterialNodeSetInputFByKey(d0, RPR_MATERIAL_INPUT_COLOR, 1,0,0,0 ));
		rpr_material_node d1 = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &d1));
		g_gc.GCAdd(d1);
		CHECK( rprMaterialNodeSetInputFByKey(d1, RPR_MATERIAL_INPUT_COLOR, 0,1,1,0 ));

		rpr_material_node blend = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_BLEND, &blend));
		g_gc.GCAdd(blend);
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR0, d0));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_COLOR1, d1));
		CHECK( rprMaterialNodeSetInputNByKey(blend, RPR_MATERIAL_INPUT_WEIGHT, dotmat)); // the checker texture is used as weight to mix the 2 colors.

		CHECK(rprShapeSetMaterial(quads.back(), blend));
	}


	//
	// Demo showing how to do some simple arithemtic on an image.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		// here we are just increasing the brightness of the image by adding +0.5 to the RGB channels.
		rpr_material_node op1 = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &op1) );
		g_gc.GCAdd(op1);
		CHECK( rprMaterialNodeSetInputUByKey(op1, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_ADD) );
		CHECK( rprMaterialNodeSetInputNByKey(op1, RPR_MATERIAL_INPUT_COLOR0, materialImage2) );
		CHECK( rprMaterialNodeSetInputFByKey(op1, RPR_MATERIAL_INPUT_COLOR1, 0.5, 0.5, 0.5, 0.0) );
	
		rpr_material_node diffuseMaterial = nullptr;
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMaterial));
		g_gc.GCAdd(diffuseMaterial);
		CHECK(rprMaterialNodeSetInputNByKey(diffuseMaterial, RPR_MATERIAL_INPUT_COLOR, op1));

		// assign the material on the shape
		CHECK(rprShapeSetMaterial(quads.back(), diffuseMaterial));
	}

	//
	// Demo of the RPR_MATERIAL_NODE_USER_TEXTURE.
	// This feature allows the RPR API user to create its own material source code.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		rpr_material_node userTexture = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_USER_TEXTURE, &userTexture) );
		g_gc.GCAdd(userTexture);
		CHECK( rprMaterialNodeSetInputUByKey(userTexture, RPR_MATERIAL_INPUT_OP, 2 )); // use the index '2' of User Texture.

		CHECK( rprMaterialNodeSetInputNByKey(userTexture, RPR_MATERIAL_INPUT_2, materialImage2 ) ); // set the 'c' input argument of the user texture
		CHECK( rprMaterialNodeSetInputNByKey(userTexture, RPR_MATERIAL_INPUT_3, uv_node ) ); // set the 'd' input argument of the user texture

		// set the index '2' of User Texture.
		// we need to provide both a string and a callback.
		// if running on GPU, the string will be used, if running on CPU, the callback is will be used.
		// Make sure to give the same source code for both.
		// using the same macro as this demo is a good pratice to define the user texture.
		CHECK( rprContextSetUserTexture(context, 2, g_str_RprCustomMatA.c_str(), (void*)RprCustomMatA ) ); 
	
		rpr_material_node diffuseMaterial = nullptr;
		CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseMaterial));
		g_gc.GCAdd(diffuseMaterial);
		CHECK(rprMaterialNodeSetInputNByKey(diffuseMaterial, RPR_MATERIAL_INPUT_COLOR, userTexture));

		// assign the material on the shape
		CHECK(rprShapeSetMaterial(quads.back(), diffuseMaterial));
		
	}

	//
	// Demo for texture wrapping.
	// The wrapping define how the texture should be render outside the [0,1] UV range. 
	// it's defined inside the RPR_MATERIAL_NODE_IMAGE_TEXTURE material.
	//
	{
		quads.push_back( AddSquare(context, scene) );

		rpr_material_node materialNodeTexture = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_IMAGE_TEXTURE, &materialNodeTexture) );
		g_gc.GCAdd(materialNodeTexture);
		CHECK( rprMaterialNodeSetInputImageDataByKey(materialNodeTexture, RPR_MATERIAL_INPUT_DATA, image1) );

		// we do some mirrored repeat along the X axis.
		CHECK( rprMaterialNodeSetInputUByKey(materialNodeTexture, RPR_MATERIAL_INPUT_WRAP_U, RPR_IMAGE_WRAP_TYPE_MIRRORED_REPEAT) );

		// Along the Y axis we clamp.
		CHECK( rprMaterialNodeSetInputUByKey(materialNodeTexture, RPR_MATERIAL_INPUT_WRAP_V, RPR_IMAGE_WRAP_TYPE_CLAMP_TO_EDGE) );

		float scaleUVx = 2.5f;
		float scaleUVy = 2.5f;

		rpr_material_node uv_node = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_INPUT_LOOKUP, &uv_node) );
		g_gc.GCAdd(uv_node);
		CHECK( rprMaterialNodeSetInputUByKey(uv_node, RPR_MATERIAL_INPUT_VALUE, RPR_MATERIAL_NODE_LOOKUP_UV) );

		rpr_material_node uvScaled = nullptr;
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_ARITHMETIC, &uvScaled) );
		g_gc.GCAdd(uvScaled);
		CHECK( rprMaterialNodeSetInputUByKey(uvScaled, RPR_MATERIAL_INPUT_OP, RPR_MATERIAL_NODE_OP_MUL) );
		CHECK( rprMaterialNodeSetInputNByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR0, uv_node) );
		CHECK( rprMaterialNodeSetInputFByKey(uvScaled, RPR_MATERIAL_INPUT_COLOR1, scaleUVx, scaleUVy, 0.0f, 0) );

		CHECK( rprMaterialNodeSetInputNByKey(materialNodeTexture, RPR_MATERIAL_INPUT_UV, uvScaled ) );

		rpr_material_node diffuseSphere = nullptr; 
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuseSphere));
		g_gc.GCAdd(diffuseSphere);
		CHECK( rprMaterialNodeSetInputNByKey(diffuseSphere, RPR_MATERIAL_INPUT_COLOR, materialNodeTexture ));

		// assign the material on the shape
		CHECK(rprShapeSetMaterial(quads.back(), diffuseSphere));
		
	}


	// Create framebuffer 
	rpr_framebuffer_desc desc = { 1200 , 800 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	g_gc.GCAdd(frame_buffer);
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	g_gc.GCAdd(frame_buffer_resolved);
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) );

	// Render the scene
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 100));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"35.png"));


	// Release the stuff we created
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}




