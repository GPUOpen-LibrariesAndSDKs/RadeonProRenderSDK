/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender SDK rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*
*  Copyright 2011 - 2017 Advanced Micro Devices, Inc. (unpublished)
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*
\*****************************************************************************/

#ifdef __APPLE__
#include "GL/glew.h"
#elif WIN32
#define NOMINMAX
#include <Windows.h>
#include "GL/glew.h"
#include "GLUT/GLUT.h"
#endif

#include "RadeonProRender.h"
#include "RadeonProRender_GL.h"
#include "Math/mathutils.h"
#include "../common/common.h"
#include "ShaderManager.h"


#ifdef __APPLE__
    #ifndef GL_RGBA32F
    #define GL_RGBA32F GL_RGBA32F_ARB
    #endif 
#endif

#include <cassert>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLuint              g_vertex_buffer_id = NULL;
GLuint              g_index_buffer_id = NULL;
GLuint              g_texture = NULL;
rpr_framebuffer		g_frame_buffer = NULL;
rpr_context         g_context = NULL;
rpr_material_system g_matsys = NULL;
rpr_light           g_light = NULL;
rpr_framebuffer     g_frame_buffer_2 = NULL;
ShaderManager       g_shader_manager;

void Update()
{
	// Send update event
	glutPostRedisplay();
}

void Display()
{
	// Render FR image into the GL texture
	rprContextRender(g_context);
	rprContextResolveFrameBuffer(g_context, g_frame_buffer, g_frame_buffer_2, false);//Resolve normalization postprocess

	// Clear backbuffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind vertex & index buffers of a quad
	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);

	// Set shaders
	GLuint program = g_shader_manager.GetProgram("../32_gl_interop/simple");
	glUseProgram(program);

	// Set texture with the image rendered by FR
	GLuint texture_loc = glGetUniformLocation(program, "g_Texture");
	CHECK_GE(texture_loc , 0);

	glUniform1i(texture_loc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	GLuint position_attr_id = glGetAttribLocation(program, "inPosition");
	GLuint texcoord_attr_id = glGetAttribLocation(program, "inTexcoord");

	glVertexAttribPointer(position_attr_id, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(texcoord_attr_id, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(position_attr_id);
	glEnableVertexAttribArray(texcoord_attr_id);

	// Draw quad with the texture on top of it
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

	glDisableVertexAttribArray(position_attr_id);
	glDisableVertexAttribArray(texcoord_attr_id);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	// Present backbuffer
	glutSwapBuffers();
}


void InitGraphics()
{
	// Set states
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glCullFace(GL_NONE);
	glDisable(GL_DEPTH_TEST);

	// Viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Create vertex and index buffer for a quad
	glGenBuffers(1, &g_vertex_buffer_id);
	glGenBuffers(1, &g_index_buffer_id);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);

	float quadVertexData[] =
	{
		-1, -1, 0.5, 0, 0,
		1, -1, 0.5, 1, 0,
		1,  1, 0.5, 1, 1,
		-1,  1, 0.5, 0, 1
	};

	GLshort quadIndexData[] =
	{
		0, 1, 3,
		3, 1, 2
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndexData), quadIndexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create a texture for FR rendering
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &g_texture);

	glBindTexture(GL_TEXTURE_2D, g_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);
}


int main(int argc, char** argv)
{
	//	enable firerender API trace
	//	set this before any fr API calls
	//	frContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

	//GL setup
	{
		// Initialize GLUT and GLEW libraries
		glutInit(&argc, (char**)argv);
		glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutCreateWindow("gl_interop");
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::cout << "GLEW initialization failed\n";
			return -1;
		}

		// Set OpenGL states
		InitGraphics();
	}

	std::cout << "FireRender SDK simple rendering tutorial.\n";
	// Indicates whether the last operation has suceeded or not
	rpr_int status = RPR_SUCCESS;
	// Create OpenCL context using a single GPU 

	// Register Tahoe ray tracing plugin.
	rpr_int tahoePluginID = rprRegisterPlugin(RPR_PLUGIN_FILE_NAME); 
	CHECK_NE(tahoePluginID , -1)
	rpr_int plugins[] = { tahoePluginID };
	size_t pluginCount = sizeof(plugins) / sizeof(plugins[0]);

	// Create context using a single GPU 
	CHECK( rprCreateContext(RPR_API_VERSION, plugins, pluginCount, RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_GL_INTEROP, NULL, NULL, &g_context));

	// Set active plugin.
	CHECK( rprContextSetActivePlugin(g_context, plugins[0]) );

	CHECK( rprContextCreateMaterialSystem(g_context, 0, &g_matsys) );

	std::cout << "Context successfully created.\n";

	// Create a scene
	rpr_scene scene=nullptr;
	CHECK( rprContextCreateScene(g_context, &scene) );

	// Create point light
	{
		CHECK( rprContextCreatePointLight(g_context, &g_light) );
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 2));
		CHECK(rprLightSetTransform(g_light, RPR_TRUE, &lightm.m00));
		CHECK( rprSceneAttachLight(scene, g_light) );
		CHECK(rprPointLightSetRadiantPower3f(g_light, 255, 241, 224));
	}

	// Create camera
	rpr_camera camera=nullptr;
	{
		CHECK( rprContextCreateCamera(g_context, &camera) );

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
	CHECK( rprContextSetScene(g_context, scene) );

	// Create cube mesh
	rpr_shape cube=nullptr;
	{
		CHECK(rprContextCreateMesh(g_context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &cube));

		// Add cube into the scene
		CHECK(rprSceneAttachShape(scene, cube));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1, 0));

		// Set the transform 
		CHECK(rprShapeSetTransform(cube, RPR_TRUE, &m.m00));
	}
	// Create plane mesh
	rpr_shape plane=nullptr;
	{
		CHECK(rprContextCreateMesh(g_context,
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
	rpr_material_node diffuse=nullptr;
	{
		CHECK(rprMaterialSystemCreateNode(g_matsys, RPR_MATERIAL_NODE_DIFFUSE, &diffuse));

		// Set diffuse color parameter to gray
		CHECK(rprMaterialNodeSetInputFByKey(diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f));

		// Set shader for cube & plane meshes
		CHECK(rprShapeSetMaterial(cube, diffuse));

		CHECK(rprShapeSetMaterial(plane, diffuse));
	}

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc;
	desc.fb_width = 800;
	desc.fb_height = 600;

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	CHECK(rprContextCreateFrameBuffer(g_context, fmt, &desc, &g_frame_buffer));

	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(g_frame_buffer));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));

	//Check 40_postprocess tutorial for thoses line
	//need to normalize before bloom
	rpr_post_effect normalizationEff = 0;
	CHECK(rprContextCreatePostEffect(g_context, RPR_POST_EFFECT_NORMALIZATION, &normalizationEff));
	CHECK(rprContextAttachPostEffect(g_context, normalizationEff));

	CHECK( rprContextSetParameterByKey1u(g_context,RPR_CONTEXT_PREVIEW, 1u ) );

	/////////// GL Interop Tutorial //////////

	// 1/ You must init GL
	// 2/ You must create the rpr_context with RPR_CREATION_FLAGS_ENABLE_GL_INTEROP
	// 3/ Create a GL_TEXTURE_2D
	// 4/ Create a FrameBuffer from GLTexture2D rprContextCreateFramebufferFromGLTexture2D from RadeonProRender_GL.h

	// Create GL Interop framebuffer to store rendering result
	CHECK(rprContextCreateFramebufferFromGLTexture2D(g_context, GL_TEXTURE_2D, 0, g_texture, &g_frame_buffer_2));
	
	// Clear framebuffer to black color
	CHECK(rprFrameBufferClear(g_frame_buffer_2));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));


	glutDisplayFunc(Display);
	glutIdleFunc(Update);
	glutMainLoop();

	//// Release the stuff we created
	//CHECK(rprObjectDelete(matsys));matsys=nullptr;
	//CHECK(rprObjectDelete(plane));plane=nullptr;
	//CHECK(rprObjectDelete(cube));cube=nullptr;
	//CHECK(rprObjectDelete(light));light=nullptr;
	//CHECK(rprObjectDelete(diffuse));diffuse=nullptr;
	//CHECK(rprObjectDelete(scene));scene=nullptr;
	//CHECK(rprObjectDelete(camera));camera=nullptr;
	//CHECK(rprObjectDelete(frame_buffer));frame_buffer=nullptr;
	//CheckNoLeak(context);
	//CHECK(rprObjectDelete(context));context=nullptr; // Always delete the RPR Context in last.
	return 0;
}


// Things to try in this tutorial:
// 1) You could try to test a CPU copy with rprFrameBufferGetInfo RPR_FRAMEBUFFER_DATA and then upload it with gl call