/*****************************************************************************\
*
*  Module Name    IES light Demo
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
// This demos shows how to use IES (Illuminating Engineering Society) lights
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
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags, NULL, NULL, &context) );

	// Set the active plugin.
	CHECK(  rprContextSetActivePlugin(context, plugins[0]) );

	std::cout << "RPR Context creation succeeded." << std::endl;

	// Create the scene. 
	rpr_scene scene = nullptr;
	CHECK( rprContextCreateScene(context, &scene) ); // create the scene
	CHECK( rprContextSetScene(context, scene) ); // set this scene as the "active" scene used for rendering.

	// Create the camera
	rpr_camera camera = nullptr;
	CHECK( rprContextCreateCamera(context, &camera) );
	CHECK( rprCameraLookAt(camera, 10, 10, 10,    0, 0, 0,    0, 1, 0) );
	CHECK( rprSceneSetCamera(scene, camera) );
	

	// create the material system
	rpr_material_system matsys = nullptr;
	CHECK( rprContextCreateMaterialSystem(context, 0, &matsys) );


	// Create the wall shape
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
		g_gc.GCAdd(plane);

		RadeonProRender::matrix mat = 
			  RadeonProRender::translation(RadeonProRender::float3(0.0f, 3.0f, -4.5f)) 
			* RadeonProRender::scale(RadeonProRender::float3(0.5f, 0.2f, 1.0f))
			* RadeonProRender::rotation_x(MY_PI/2.0);

		CHECK(rprShapeSetTransform(plane, RPR_TRUE, &mat.m00));

		CHECK(rprSceneAttachShape(scene, plane));
	}


	// create the floor
	CHECK( CreateAMDFloor(context, scene, matsys, g_gc, 0.5f, 0.5f)  );

	//
	// create the red IES light from a IES file data
	//
	rpr_light lightA = NULL; 
	CHECK( rprContextCreateIESLight(context, &lightA)  ); 
	g_gc.GCAdd(lightA);
	CHECK( rprObjectSetName(lightA,"my_light_A")  );
	CHECK( rprIESLightSetImageFromFile(lightA, "../../Resources/Textures/ies_light_1.ies", 256, 256)  );
	CHECK( rprIESLightSetRadiantPower3f(lightA, 3.0f, 0.0f, 0.0f)  );
	RadeonProRender::matrix lightAm = RadeonProRender::translation(RadeonProRender::float3(-3.0, 4.0, -4.4)) *  RadeonProRender::rotation_z( 0.0 * MY_PI/2.0f ) ;
	CHECK( rprLightSetTransform(lightA, true, &lightAm.m00)  );
	CHECK( rprSceneAttachLight(scene, lightA)  );

	//
	// create the green IES light from a buffer data
	//
	rpr_light lightB = NULL; 
	CHECK( rprContextCreateIESLight(context, &lightB)  );
	g_gc.GCAdd(lightB);
	CHECK( rprObjectSetName(lightB,"my_light_B")  );
	const char iesFileB[] = 
	"IESNA91\n\
	TILT=NONE\n\
	1 -1.00 1.00 213 2 1 2 0.00 0.00 0.00\n\
	1.0 1.0 0.0\n\
	\n\
	0.00 1.00 2.00 3.00 4.00 5.00 5.29 6.00 7.00 8.00 \n\
	 9.00 10.00 10.59 11.00 12.00 13.00 14.00 15.00 15.88 16.00 \n\
	 17.00 18.00 19.00 20.00 21.00 21.18 22.00 23.00 24.00 25.00 \n\
	 26.00 26.47 27.00 28.00 29.00 30.00 31.00 31.76 32.00 33.00 \n\
	 34.00 35.00 36.00 37.00 37.06 38.00 39.00 40.00 41.00 42.00 \n\
	 42.35 43.00 44.00 45.00 46.00 47.00 47.65 48.00 49.00 50.00 \n\
	 51.00 52.00 52.94 53.00 54.00 55.00 56.00 57.00 58.00 58.24 \n\
	 59.00 60.00 61.00 62.00 63.00 63.53 64.00 65.00 66.00 67.00 \n\
	 68.00 68.82 69.00 70.00 71.00 72.00 73.00 74.00 74.12 75.00 \n\
	 76.00 77.00 78.00 79.00 79.41 80.00 81.00 82.00 83.00 84.00 \n\
	 84.71 85.00 86.00 87.00 88.00 89.00 90.00 91.00 92.00 93.00 \n\
	 94.00 95.00 95.29 96.00 97.00 98.00 99.00 100.00 100.59 101.00 \n\
	 102.00 103.00 104.00 105.00 105.88 106.00 107.00 108.00 109.00 110.00 \n\
	 111.00 111.18 112.00 113.00 114.00 115.00 116.00 116.47 117.00 118.00 \n\
	 119.00 120.00 121.00 121.76 122.00 123.00 124.00 125.00 126.00 127.00 \n\
	 127.06 128.00 129.00 130.00 131.00 132.00 132.35 133.00 134.00 135.00 \n\
	 136.00 137.00 137.65 138.00 139.00 140.00 141.00 142.00 142.94 143.00 \n\
	 144.00 145.00 146.00 147.00 148.00 148.24 149.00 150.00 151.00 152.00 \n\
	 153.00 153.53 154.00 155.00 156.00 157.00 158.00 158.82 159.00 160.00 \n\
	 161.00 162.00 163.00 164.00 164.12 165.00 166.00 167.00 168.00 169.00 \n\
	 169.41 170.00 171.00 172.00 173.00 174.00 174.71 175.00 176.00 177.00 \n\
	 178.00 179.00 180.00 \n\
	\n\
	0.00 360.00 \n\
	\n\
	12.00 12.09 12.18 12.24 12.25 12.21 12.19 12.72 13.39 13.98 \n\
	 14.49 14.91 15.10 14.10 11.73 9.47 7.34 5.35 3.74 3.90 \n\
	 5.14 6.19 7.04 7.69 8.14 8.21 7.12 5.90 4.81 3.84 \n\
	 3.00 2.65 2.39 1.94 1.54 1.20 0.91 0.72 0.73 0.74 \n\
	 0.65 0.51 0.34 0.17 0.17 0.04 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 \n\
	\n\
	12.00 12.09 12.18 12.24 12.25 12.21 12.19 12.72 13.39 13.98 \n\
	 14.49 14.91 15.10 14.10 11.73 9.47 7.34 5.35 3.74 3.90 \n\
	 5.14 6.19 7.04 7.69 8.14 8.21 7.12 5.90 4.81 3.84 \n\
	 3.00 2.65 2.39 1.94 1.54 1.20 0.91 0.72 0.73 0.74 \n\
	 0.65 0.51 0.34 0.17 0.17 0.04 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 0.00 \n\
	 0.00 0.00 0.00 ";
	CHECK( rprIESLightSetImageFromIESdata(lightB, iesFileB, 256, 256)  );
	CHECK( rprIESLightSetRadiantPower3f(lightB, 0.0f, 3.0f, 0.0f)  );
	RadeonProRender::matrix lightBm = RadeonProRender::translation(RadeonProRender::float3(+3.0, 4.0, -4.0)) ;
	CHECK( rprLightSetTransform(lightB, true, &lightBm.m00)  );
	CHECK( rprSceneAttachLight(scene, lightB)  );


	//
	// create the bleu IES light from a IES file data
	//
	rpr_light lightC = NULL; 
	CHECK( rprContextCreateIESLight(context, &lightC)  );
	g_gc.GCAdd(lightC);
	CHECK( rprObjectSetName(lightC,"my_light_B")  );
	CHECK( rprIESLightSetImageFromFile(lightC, "../../Resources/Textures/ies_light_2.ies", 256, 256)  );
	CHECK( rprIESLightSetRadiantPower3f(lightC, 0.0f, 0.0f, 2.0f)  );
	RadeonProRender::matrix lightCm = RadeonProRender::translation(RadeonProRender::float3(0.0, 0.3, 4.0)) *  RadeonProRender::rotation_x( 0.8 * MY_PI/2.0f ) ;
	CHECK( rprLightSetTransform(lightC, true, &lightCm.m00)  );
	CHECK( rprSceneAttachLight(scene, lightC)  );


	// Create framebuffer 
	rpr_framebuffer_desc desc = { 800 , 600 }; 
	rpr_framebuffer_format fmt = {4, RPR_COMPONENT_TYPE_FLOAT32}; 
	rpr_framebuffer frame_buffer = nullptr;
	rpr_framebuffer frame_buffer_resolved = nullptr;
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer) );
	CHECK( rprContextCreateFrameBuffer(context, fmt, &desc, &frame_buffer_resolved) );
	CHECK( rprContextSetAOV(context, RPR_AOV_COLOR, frame_buffer) );

	// for this rendering, we need some radiance clamp: that avoids to have some light noise from the lights
	CHECK( rprContextSetParameterByKey1f(context,RPR_CONTEXT_RADIANCE_CLAMP, 2.0f ) );

	// set rendering gamma
	CHECK( rprContextSetParameterByKey1f(context, RPR_CONTEXT_DISPLAY_GAMMA , 2.5f ) );

	// Render the scene
	CHECK(rprContextSetParameterByKey1u(context,RPR_CONTEXT_ITERATIONS, 500));
	CHECK( rprContextRender(context) );
	CHECK(rprContextResolveFrameBuffer(context,frame_buffer,frame_buffer_resolved,false));
	CHECK(rprFrameBufferSaveToFile(frame_buffer_resolved,"28.png"));


	// Release the stuff we created
	CHECK(rprObjectDelete(camera)); camera=nullptr;
	CHECK(rprObjectDelete(frame_buffer)); frame_buffer=nullptr;
	CHECK(rprObjectDelete(frame_buffer_resolved)); frame_buffer_resolved=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(scene)); scene=nullptr;
	CHECK(rprObjectDelete(matsys)); matsys=nullptr;
	CheckNoLeak(context);
	CHECK(rprObjectDelete(context)); context=nullptr;
	return 0;
}




