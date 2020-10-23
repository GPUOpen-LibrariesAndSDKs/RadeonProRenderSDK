/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright 2011 - 2020 Advanced Micro Devices, Inc.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "Math/mathutils.h"
#include "RprLoadStore.h"//For Import
#include "../common/common.h"

#include <cassert>
#include <iostream>

const int nbIterations = 300;

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any fr API calls
	//	frContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK matball scene rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create RPR context using a single GPU 
	rpr_context context = nullptr;

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME);
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK(rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context));

	// Set active plugin.
	CHECK(rprContextSetActivePlugin(context, plugins[0]));


	rpr_material_system matsys = nullptr;
	CHECK(rprContextCreateMaterialSystem(context, 0, &matsys));
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK(rprContextCreateScene(context, &scene));
	// Set scene to render for the context
	CHECK(rprContextSetScene(context, scene));

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 1000, 1000 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	rpr_framebuffer frame_buffer = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer));
	rpr_framebuffer frame_buffer_resolve = nullptr;
	CHECK(rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolve));

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(frame_buffer));
	CHECK(rprFrameBufferClear(frame_buffer_resolve));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer));

	/////////Mesh Import//////////
	CHECK(rprsImport("../../Resources/Meshes/matball.rprs", context, matsys, &scene, true));

	// Progressively render an image
	std::cout << "Rendering scene 1... ";
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,nbIterations));
	CHECK(rprContextRender(context));
	std::cout << "finished.\n";

	// apply post effect on 'frame_buffer' ,  store result in 'frame_buffer_resolve'
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolve,false));

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolve, "62_1.png"));


	//
	// Now, we are going to modify the original 'matball.rprs' scene, and change the materials
	//

	// enumerate the shapes in the scene, and find the 3 shapes corresponding to the matball object.
	std::vector<rpr_shape> shapesToChangeMaterial;
	{
		int nbOfObjs = 0;
		CHECK(rprsListImportedShapes(0,0,&nbOfObjs)); // list the shapes loaded by last rprsImport call
		rpr_shape* objs = new rpr_shape[nbOfObjs];
		CHECK(rprsListImportedShapes(objs,nbOfObjs*sizeof(rpr_shape),0));
		for(int i=0; i<nbOfObjs; i++)
		{
			char name[2048];
			CHECK(rprShapeGetInfo(objs[i], RPR_SHAPE_NAME, sizeof(name), name, nullptr));

			if (   strcmp(name,"Probe01") == 0
				|| strcmp(name,"Probe02") == 0
				|| strcmp(name,"Probe00") == 0
				)
			{
				shapesToChangeMaterial.push_back(objs[i]);
			}
		}
		delete[] objs; objs = NULL;
	}


	// create a new Uber Material

	rpr_material_node uberMat2 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat2));

	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS   ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,0.996078, 0.858824, 0.639216, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR   ,0.996078, 0.858824, 0.639216, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT    ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR    ,1.36, 1.36, 1.36, 1.36));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE   ,0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR    ,0.996078, 0.858824, 0.639216, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS   ,0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_COLOR  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_MODE     ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_IOR     ,3, 3, 3, 3));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_METALNESS   ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR     ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SHEEN    ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SHEEN_TINT   ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_EMISSION_MODE    ,RPR_UBER_MATERIAL_EMISSION_MODE_SINGLESIDED));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_TRANSPARENCY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT   ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER    ,0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR   ,1, 1, 1, 1));

	// Apply this new Uber Material to the shapes
	for(const auto& iShape : shapesToChangeMaterial)
	{
		CHECK(rprShapeSetMaterial(iShape,uberMat2));
	}

	// clean framebuffer
	CHECK(rprFrameBufferClear(frame_buffer));

	// Progressively render an image
	std::cout << "Rendering scene 2... ";
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,nbIterations));
	CHECK(rprContextRender(context));
	std::cout << "finished.\n";

	// apply post effect on 'frame_buffer' ,  store result in 'frame_buffer_resolve'
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolve,false));

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolve, "62_2.png"));




	// create a new Uber Material

	rpr_image uberMat3_img = nullptr;
	CHECK(rprContextCreateImageFromFile(context,"../../Resources/Textures/brass standard_bump.png",&uberMat3_img));

	rpr_material_node uberMat3_imgTexture = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat3_imgTexture));
	CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat3_imgTexture,   RPR_MATERIAL_INPUT_DATA  ,uberMat3_img));

	rpr_material_node uberMat3_bumpmap = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_BUMP_MAP,&uberMat3_bumpmap));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat3_bumpmap,   RPR_MATERIAL_INPUT_COLOR  ,uberMat3_imgTexture));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3_bumpmap,   RPR_MATERIAL_INPUT_SCALE  ,0.2, 0.2, 0.2, 0.2));

	rpr_material_node uberMat3 = nullptr;
	CHECK(rprMaterialSystemCreateNode(matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat3));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR  ,0.937255, 0.870588, 0.576471, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT   ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_DIFFUSE_ROUGHNESS  ,0.5, 0.5, 0.5, 0.5));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL ,uberMat3_bumpmap));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR    ,0.937255, 0.870588, 0.576471, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT     ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS   ,0.1, 0.1, 0.1, 0.1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION   ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE    ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR    ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputNByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL   ,uberMat3_bumpmap));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_COLOR  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_ROUGHNESS    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_IOR   ,1.5, 1.5, 1.5, 1.5));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_THIN_SURFACE   ,0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_COLOR  ,1, 1, 1, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_ABSORPTION_DISTANCE   ,1, 1, 1, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_REFRACTION_CAUSTICS    ,1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_COLOR    ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_ROUGHNESS   ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_MODE     ,RPR_UBER_MATERIAL_IOR_MODE_PBR));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_IOR     ,3, 3, 3, 3));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_METALNESS     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_TRANSMISSION_COLOR  ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_COATING_THICKNESS     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SHEEN     ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SHEEN_TINT     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SHEEN_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_EMISSION_COLOR     ,1, 1, 1, 1));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_EMISSION_WEIGHT    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_EMISSION_MODE     ,RPR_UBER_MATERIAL_EMISSION_MODE_SINGLESIDED));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_TRANSPARENCY    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_COLOR     ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DISTANCE    ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SSS_SCATTER_DIRECTION  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SSS_WEIGHT      ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputUByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_SSS_MULTISCATTER   ,0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_WEIGHT  ,0, 0, 0, 0));
	CHECK(rprMaterialNodeSetInputFByKey(uberMat3, RPR_MATERIAL_INPUT_UBER_BACKSCATTER_COLOR    ,0.937255, 0.870588, 0.576471, 0));

	// Apply this new Uber Material to the shapes
	for(const auto& iShape : shapesToChangeMaterial)
	{
		CHECK(rprShapeSetMaterial(iShape,uberMat3));
	}

	CHECK(rprFrameBufferClear(frame_buffer));

	// Progressively render an image
	std::cout << "Rendering scene 3... ";
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,nbIterations));
	CHECK(rprContextRender(context));
	std::cout << "finished.\n";

	// apply post effect on 'frame_buffer' ,  store result in 'frame_buffer_resolve'
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolve,false));

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolve, "62_3.png"));


	//
	// Cleaning
	//

	// delete the RPR objects created during the last rprsImport call.
	CHECK(rprsDeleteListImportedObjects(nullptr));

	// Release the stuff we created
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(uberMat2));uberMat2=nullptr;
	CHECK(rprObjectDelete(uberMat3));uberMat3=nullptr;
	CHECK(rprObjectDelete(uberMat3_bumpmap));uberMat3_bumpmap=nullptr;
	CHECK(rprObjectDelete(uberMat3_img));uberMat3_img=nullptr;
	CHECK(rprObjectDelete(uberMat3_imgTexture));uberMat3_imgTexture=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolve));frame_buffer_resolve=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}

