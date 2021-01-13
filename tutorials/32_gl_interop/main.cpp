/*****************************************************************************\
*
*  Module Name    simple_render.cpp
*  Project        Radeon ProRender SDK rendering tutorial
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
rpr_scene			g_scene=nullptr;
rpr_shape			g_cube=nullptr;
rpr_camera			g_camera=nullptr;
rpr_material_node	g_diffuse=nullptr;
rpr_shape			g_plane=nullptr;
float*				g_fbdata = nullptr;
float				g_camera_posX = 0.0;
float				g_camera_posY = 5.0;
int					g_lastMouseDownUpdateX = -1;
int					g_lastMouseDownUpdateY = -1;

void Update()
{
	// Send update event
	glutPostRedisplay();
}

void MoveCamera()
{
	CHECK( rprCameraLookAt(
		g_camera, g_camera_posX, g_camera_posY, 20, // position
		0, 1, 0, // look at point
		0, 1, 0  // up vector
		) );

	// camera moved, so we need to redraw the framebuffer.
	CHECK( rprFrameBufferClear(g_frame_buffer) );
}

void OnMouseMoveEvent(int x, int y)
{
	g_camera_posX += (x-g_lastMouseDownUpdateX)/4;
	g_camera_posY += (y-g_lastMouseDownUpdateY)/4;

	// avoid to have a camera under the floor.
	if ( g_camera_posY < 0.1 ) { g_camera_posY = 0.1; }

	g_lastMouseDownUpdateX = x;
	g_lastMouseDownUpdateY = y;

	MoveCamera();
	return;
}

void OnMouseEvent(int button, int state, int x, int y)
{
	if ( button ==  GLUT_LEFT_BUTTON )
	{
		if ( state == GLUT_DOWN )
		{
			g_lastMouseDownUpdateX = x;
			g_lastMouseDownUpdateY = y;
		}
		else if ( state == GLUT_UP )
		{
		}
	}

	return;
}

void OnKeyboardEvent(unsigned char key, int xmouse, int ymouse)
{	
	bool cameraMoves = false;
	switch (key)
	{
		case 'w':
		case 'z':
		{
			g_camera_posX += 0.5f;
			cameraMoves = true;
			break;
		}

		case 's':
		{
			g_camera_posX -= 0.5f;
			cameraMoves = true;
			break;
		}

		default:
			break;
	}

	if ( cameraMoves )
		MoveCamera();
	
}

void Display()
{
	// Render FR image into the GL texture
	rprContextRender(g_context);
	rprContextResolveFrameBuffer(g_context, g_frame_buffer, g_frame_buffer_2, true);

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



	CHECK(rprFrameBufferGetInfo(g_frame_buffer_2, RPR_FRAMEBUFFER_DATA, WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(float)*4, g_fbdata, NULL));

	//glBindTexture(GL_TEXTURE_2D, g_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_FLOAT, static_cast<const GLvoid*>(g_fbdata));         
	//glBindTexture(GL_TEXTURE_2D, 0);



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

void OnExit()
{
	//// Release the stuff we created
	std::cout <<"Release memory...\n";
	
	CHECK(rprObjectDelete(g_scene));g_scene=nullptr;
	CHECK(rprObjectDelete(g_cube));g_cube=nullptr;
	CHECK(rprObjectDelete(g_camera));g_camera=nullptr;
	CHECK(rprObjectDelete(g_diffuse));g_diffuse=nullptr;
	CHECK(rprObjectDelete(g_plane));g_plane=nullptr;
	CHECK(rprObjectDelete(g_matsys));g_matsys=nullptr;
	CHECK(rprObjectDelete(g_light));g_light=nullptr;
	CHECK(rprObjectDelete(g_frame_buffer));g_frame_buffer=nullptr;
	CHECK(rprObjectDelete(g_frame_buffer_2));g_frame_buffer_2=nullptr;
	CheckNoLeak(g_context);
	CHECK(rprObjectDelete(g_context));g_context=nullptr; // Always delete the RPR Context in last.
	delete[] g_fbdata; g_fbdata=nullptr;
}

int main(int argc, char** argv)
{
	//	enable firerender API trace
	//	set this before any RPR API calls
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
	status = rprCreateContext(RPR_API_VERSION, plugins, pluginCount, g_ContextCreationFlags 
		#ifndef USING_NORTHSTAR   // note that for Northstar, we don't need the GL_INTEROP flag
		| RPR_CREATION_FLAGS_ENABLE_GL_INTEROP
		#endif
		, NULL, NULL, &g_context);
	CHECK(status);

	// Set active plugin.
	CHECK( rprContextSetActivePlugin(g_context, plugins[0]) );

	CHECK( rprContextCreateMaterialSystem(g_context, 0, &g_matsys) );

	std::cout << "Context successfully created.\n";

	// Create a scene
	CHECK( rprContextCreateScene(g_context, &g_scene) );

	// Create point light
	{
		CHECK( rprContextCreatePointLight(g_context, &g_light) );
		RadeonProRender::matrix lightm = RadeonProRender::translation(RadeonProRender::float3(0, 8, 2));
		CHECK(rprLightSetTransform(g_light, RPR_TRUE, &lightm.m00));
		CHECK( rprSceneAttachLight(g_scene, g_light) );
		CHECK(rprPointLightSetRadiantPower3f(g_light, 255, 241, 224));
	}

	// Create camera
	{
		CHECK( rprContextCreateCamera(g_context, &g_camera) );

		// Position camera in world space: 
		CHECK( rprCameraLookAt(g_camera, g_camera_posX, g_camera_posY, 20, 0, 1, 0, 0, 1, 0) );

		CHECK( rprCameraSetFocalLength(g_camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(g_scene, g_camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(g_context, g_scene) );

	// Create cube mesh
	{
		CHECK(rprContextCreateMesh(g_context,
			(rpr_float const*)&cube_data[0], 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 3), 24, sizeof(vertex),
			(rpr_float const*)((char*)&cube_data[0] + sizeof(rpr_float) * 6), 24, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 12, &g_cube));

		// Add cube into the scene
		CHECK(rprSceneAttachShape(g_scene, g_cube));

		// Create a transform: -2 unit along X axis and 1 unit up Y axis
		RadeonProRender::matrix m = RadeonProRender::translation(RadeonProRender::float3(-2, 1, 0));

		// Set the transform 
		CHECK(rprShapeSetTransform(g_cube, RPR_TRUE, &m.m00));
	}
	// Create plane mesh
	{
		CHECK(rprContextCreateMesh(g_context,
			(rpr_float const*)&plane_data[0], 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 3), 4, sizeof(vertex),
			(rpr_float const*)((char*)&plane_data[0] + sizeof(rpr_float) * 6), 4, sizeof(vertex),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			(rpr_int const*)indices, sizeof(rpr_int),
			num_face_vertices, 2, &g_plane));

		// Add plane into the scene
		CHECK(rprSceneAttachShape(g_scene, g_plane));
	}

	// Create simple diffuse shader
	{
		CHECK(rprMaterialSystemCreateNode(g_matsys, RPR_MATERIAL_NODE_DIFFUSE, &g_diffuse));

		// Set diffuse color parameter to gray
		CHECK(rprMaterialNodeSetInputFByKey(g_diffuse, RPR_MATERIAL_INPUT_COLOR, 0.5f, 0.5f, 0.5f, 1.f));

		// Set shader for cube & plane meshes
		CHECK(rprShapeSetMaterial(g_cube, g_diffuse));

		CHECK(rprShapeSetMaterial(g_plane, g_diffuse));
	}

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { WINDOW_WIDTH,WINDOW_HEIGHT };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	CHECK(rprContextCreateFrameBuffer(g_context, fmt, &desc, &g_frame_buffer));
	CHECK(rprContextCreateFrameBuffer(g_context, fmt, &desc, &g_frame_buffer_2));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));

	CHECK( rprContextSetParameterByKey1u(g_context,RPR_CONTEXT_PREVIEW, 1u ) );

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));

	g_fbdata = new float[WINDOW_WIDTH * WINDOW_HEIGHT * 4];


	std::cout << "Press W or S to move the camera.\n";

	glutDisplayFunc(Display);
	glutIdleFunc(Update);
	glutKeyboardFunc(OnKeyboardEvent);
	glutMouseFunc(OnMouseEvent);
	glutMotionFunc(OnMouseMoveEvent);
	glutMainLoop();


	// this should be called when the OpenGL Application closes
	// ( note that it may not be called, as GLUT doesn't have "Exit" callback )
	OnExit();


	return 0;
}


