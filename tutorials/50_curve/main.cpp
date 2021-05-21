/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/
#include "RadeonProRender.h"
#include "RprLoadStore.h"
#include "Math/mathutils.h"
#include "../common/common.h"

#include <cassert>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

float draw()
{
	return (rand()&RAND_MAX)/(double)RAND_MAX;
}

int main()
{
	//	enable Radeon ProRender API trace
	//	set this before any RPR API calls
	//	frContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	std::cout << "Radeon ProRender SDK simple rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create OpenCL context using a single GPU 
	rpr_context context = NULL;

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );


	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );
	// Check if it is created successfully
	if (status != RPR_SUCCESS)
	{
		std::cout << "Context creation failed: check your OpenCL runtime and driver versions.\n";
		return -1;
	}

	std::cout << "Context successfully created.\n";

	// Create a scene
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) );

	// Create point light
	rpr_light light = nullptr;
	{
		CHECK(rprContextCreatePointLight(context, &light));

		// Create a transform: move 5 units in X axis, 8 units up Y axis, -2 units in Z axis
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 3));

		// Set transform for the light
		CHECK(rprLightSetTransform(light, RPR_TRUE, &lightm.m00));

		// Set light radiant power in Watts
		CHECK(rprPointLightSetRadiantPower3f(light, 255, 241, 224));

		// Attach the light to the scene
		CHECK(rprSceneAttachLight(scene, light));
	}
	// Create camera
	rpr_camera camera = nullptr;
	{
		CHECK( rprContextCreateCamera(context, &camera) );

		// Position camera in world space: 
		// Camera position is (5,5,20)
		// Camera aimed at (0,0,0)
		// Camera up vector is (0,1,0)
		CHECK( rprCameraLookAt(camera, 0, 5, 20, 0, 1, 0, 0, 1, 0) );

		CHECK( rprCameraSetFocalLength(camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(scene, camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(context, scene) );

	// Create plane mesh
	rpr_shape plane = nullptr;
	{
		CHECK(rprContextCreateMesh(context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &plane));

		// Add plane into the scene
		CHECK(rprSceneAttachShape(scene, plane));
	}

	// Create simple diffuse shader
	rpr_material_node diffuse = nullptr;
	{
		CHECK( rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse) );

		// Set diffuse color parameter to gray
		CHECK( rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f) );

		// Set shader for cube & plane meshes
		CHECK( rprShapeSetMaterial(plane, diffuse) );
	}
	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { 800,600 };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32};
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );

	// Clear framebuffer to black color
	CHECK( rprFrameBufferClear(frame_buffer) );

	// Set framebuffer for the context
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );


	/////////Curve Tutorial//////////
	
	//It's BCurve model of 4 point
	//It works with StartPoint CtrlPoint CtrlPoint EndPoint
	rpr_curve newCurve = 0;
	
	int nPoints = 0;
	const int n = 12;
	std::vector<float> cps;
	std::vector<rpr_uint> indices;
	std::vector<float> radius;
	std::vector<rpr_int> segsPerCurve;
	std::vector<float> uvs;
	for(int j=0; j<n; j++) for(int i=0; i<n; i++)
	{
		const float width = 5.f;
		float x = (i/(float)n - 0.5f)*width;
		float z = (j/(float)n - 0.5f)*width;

		const float length = 0.1f;
		int nSegs = 8;
		float x0 = x + (draw()-0.5f)*width/n/2;
		float z0 = z + (draw()-0.5f)*width/n/2;
		for(int l=0; l<nSegs; l++)
		{
			for(int k=0; k<4; k++)
			{
				if( k!=0 )
				{
					x0 = x + (draw()-0.5f)*width/n/2;
					z0 = z + (draw()-0.5f)*width/n/2;
				}
				cps.push_back( x0 );
				cps.push_back( (3*l + k) * length );
				cps.push_back( z0 );

				indices.push_back( indices.size() );
				nPoints++;
			}
		}
		radius.push_back( 0.01f );
		segsPerCurve.push_back( nSegs );
		uvs.push_back( i/(float)n );
		uvs.push_back( j/(float)n );
	}
	CHECK(rprContextCreateCurve(
		context,
		&newCurve,
		nPoints, // number of 3d points
		cps.data(), // list of 3d points
		3 * sizeof(float), // stride of 3d points
		indices.size(), // number of indices
		segsPerCurve.size(),	// this  rpr_curve has N curves	
		indices.data(), // indices pointing to controPoint
		radius.data(), // list of radius (float) for each curve
		uvs.data(),     // list of uv (float2) for each curve
		segsPerCurve.data(), // number of segment ( rpr_int ) for each curve
		0
	));

	CHECK(rprObjectSetName(newCurve, "myCurve0"));

	rpr_material_node newDiffuse_red = NULL;
	CHECK(rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_DIFFUSE, &newDiffuse_red));
	CHECK(rprMaterialNodeSetInputFByKey(newDiffuse_red, RPR_MATERIAL_INPUT_COLOR, 1.0f, 0.5f, 0.5f, 1.f));
	CHECK(rprCurveSetMaterial(newCurve, newDiffuse_red));

	CHECK(rprSceneAttachCurve(scene, newCurve));


	// Progressively render an image
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS,NUM_ITERATIONS));
	CHECK(rprContextRender(context));
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,true));

	std::cout << "Rendering finished.\n";

	// Save the result to file
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved, "50.png"));

	// Release the stuff we created
	CHECK(rprObjectDelete(newDiffuse_red));newDiffuse_red=nullptr;
	CHECK(rprObjectDelete(matsys));matsys=nullptr;
	CHECK(rprObjectDelete(plane));plane=nullptr;
	CHECK(rprObjectDelete(newCurve));newCurve=nullptr;
	CHECK(rprObjectDelete(light));light=nullptr;
	CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	CHECK(rprObjectDelete(scene));scene=nullptr;
	CHECK(rprObjectDelete(camera));camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved));frame_buffer_resolved=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// 1) As you probably notice the curve is made out of cylinder which produce small hole, so you should make sure your curve has enough control point to avoid this effect
// 2) You can try to add more point, change radius, add more curve, ...