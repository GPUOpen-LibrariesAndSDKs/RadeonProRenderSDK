/*****************************************************************************\
*
*  Module Name    gl_interop.cpp
*  Project        Radeon ProRender SDK rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials 
*                 Demo of an OpenGL window rendering RPR.
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/



//
// Demo covering an RPR rendering inside an OpenGL app.
// rotate camera with mouse left-click + move
// move camera with W/A/S/D keys
// press X key to exit
//


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
#include <thread>
#include <memory>

const unsigned int WINDOW_WIDTH = 640;
const unsigned int WINDOW_HEIGHT = 480;

void Display();
void OnExit();

class GuiRenderImpl
{
public:
	struct Update
	{
		Update()
		{
			clear();
			m_progress = 0.0f;
		}

		volatile int m_hasUpdate;
		volatile int m_done;
		volatile int m_aborted;
		int m_camUpdated;
		float m_progress;
		
		void clear()
		{
			m_hasUpdate = m_done = m_aborted = m_camUpdated = 0;
		}
	};
	static
	void notifyUpdate( float x, void* userData )
	{
		Update* demo = (Update*)userData;
		demo->m_hasUpdate = 1;
		demo->m_progress = x;
	}
};


GLuint              g_vertex_buffer_id = 0;
GLuint              g_index_buffer_id = 0;
GLuint              g_texture = 0;
rpr_framebuffer		g_frame_buffer = NULL;
rpr_context         g_context = NULL;
rpr_material_system g_matsys = NULL;
rpr_framebuffer     g_frame_buffer_2 = NULL;
ShaderManager       g_shader_manager;
rpr_scene			g_scene=nullptr;
rpr_camera			g_camera=nullptr;
std::shared_ptr<float>	g_fbdata = nullptr;
RPRGarbageCollector g_gc;
bool                g_askExit = false; // push X key to exit



GuiRenderImpl::Update g_update;
const auto g_invalidTime = std::chrono::time_point<std::chrono::high_resolution_clock>::max();
int					g_benchmark_numberOfRenderIteration = 0;
auto g_benchmark_start = g_invalidTime;




struct MOUSE_DRAG_INFO
{
	MOUSE_DRAG_INFO()
	{
		leftMouseButtonDown = false;
		mousePosAtMouseButtonDown_X = -1;
		mousePosAtMouseButtonDown_Y = -1;
	}

	RadeonProRender::float3 lookat;
	RadeonProRender::float3 up;
	RadeonProRender::float3 pos;
	RadeonProRender::matrix mat;

	int	mousePosAtMouseButtonDown_X;
	int	mousePosAtMouseButtonDown_Y;

	bool leftMouseButtonDown;
};
MOUSE_DRAG_INFO g_mouse_camera;



// High batch size will increase the RPR performance ( rendering iteration per second ), but lower the render feedback FPS on the OpenGL viewer.
// Note that for better OpenGL FPS (and decreased RPR render quality), API user can also tune the `RPR_CONTEXT_PREVIEW` value.
const int			g_batchSize = 15;


// thread rendering 'g_batchSize' iteration(s)
void renderJob( rpr_context ctxt, GuiRenderImpl::Update* update )
{
	CHECK( rprContextRender( ctxt ) );
	update->m_done = 1;
	return;
}


void Update()
{
	const auto timeUpdateStarts = std::chrono::high_resolution_clock::now();


	//
	// print render stats every ~100 iterations.
	//
	if ( g_benchmark_start == g_invalidTime )
		g_benchmark_start = timeUpdateStarts;
	if ( g_benchmark_numberOfRenderIteration >= 100 )
	{
		double elapsed_time_ms = std::chrono::duration<double, std::milli>(timeUpdateStarts - g_benchmark_start).count();
		double renderPerSecond = (double)g_benchmark_numberOfRenderIteration * 1000.0 / elapsed_time_ms;
		std::cout<<renderPerSecond<<" iterations per second."<<std::endl;
		g_benchmark_numberOfRenderIteration = 0;
		g_benchmark_start = timeUpdateStarts;

	}


	// clear state
	g_update.clear();

	// start the rendering thread
	std::thread t( &renderJob, g_context, &g_update );

	// wait the rendering thread
	while( !g_update.m_done )
	{
		// at each update of the rendering thread
		if( g_update.m_hasUpdate )
		{
			// Read the frame buffer from RPR
			// Note that rprContextResolveFrameBuffer and rprFrameBufferGetInfo(fb,RPR_FRAMEBUFFER_DATA)  can be called asynchronous while rprContextRender is running.

			CHECK( rprContextResolveFrameBuffer( g_context, g_frame_buffer, g_frame_buffer_2, false ) );
			size_t frame_buffer_dataSize = 0;
			CHECK( rprFrameBufferGetInfo( g_frame_buffer_2, RPR_FRAMEBUFFER_DATA, 0 , NULL , &frame_buffer_dataSize ) );

			// check that the size fits with original buffer alloc
			if ( frame_buffer_dataSize != WINDOW_WIDTH * WINDOW_HEIGHT * 4 * sizeof(float) )
			{
				CHECK(RPR_ERROR_INTERNAL_ERROR)
			}

			CHECK( rprFrameBufferGetInfo( g_frame_buffer_2, RPR_FRAMEBUFFER_DATA, frame_buffer_dataSize , g_fbdata.get() , NULL ) );

			// update the OpenGL texture with the new image from RPR
			glBindTexture(GL_TEXTURE_2D, g_texture);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_FLOAT, static_cast<const GLvoid*>(g_fbdata.get()));         
			glBindTexture(GL_TEXTURE_2D, 0);

			// clear the update flag
			g_update.m_hasUpdate = false;
		}
	
		// Request a new OpenGL Display call.
		// Note from documentation: "Multiple calls to glutPostRedisplay before the next display callback opportunity generates only a single redisplay callback".
		// So we are not actually doing the OpenGL render in this Update loop. It would be a bad design as it would stress this thread and may reduce performance of the 
		//   `renderJob` thread which is the most important here.
		glutPostRedisplay();
	
	}

	// wait the end of the rendering thread
	t.join();

	if ( g_askExit )
	{
		OnExit();
		std::exit(EXIT_SUCCESS);
	}

	g_benchmark_numberOfRenderIteration += g_batchSize;

	return;
}

void OnMouseMoveEvent(int x, int y)
{
	int delaX =  (x - g_mouse_camera.mousePosAtMouseButtonDown_X);
	int delaY = -(y - g_mouse_camera.mousePosAtMouseButtonDown_Y);

	if ( g_mouse_camera.leftMouseButtonDown )
	{
		RadeonProRender::matrix rotZ = RadeonProRender::rotation(g_mouse_camera.up, (float)delaX * 0.001);

		RadeonProRender::float3 lookAtVec = g_mouse_camera.lookat - g_mouse_camera.pos;
		lookAtVec.normalize();

		RadeonProRender::float3 left = RadeonProRender::cross(g_mouse_camera.up, lookAtVec);
		left.normalize();

		RadeonProRender::matrix rotleft = RadeonProRender::rotation(left, (float)delaY * 0.001);
		RadeonProRender::matrix newMat =  rotleft * rotZ * g_mouse_camera.mat ;

		rprCameraSetTransform( g_camera, false, &newMat.m00 );
	}

	// camera moved, so we need to redraw the framebuffer.
	CHECK( rprFrameBufferClear(g_frame_buffer) );

	return;
}

void OnMouseEvent(int button, int state, int x, int y)
{
	if ( button ==  GLUT_LEFT_BUTTON )
	{
		if ( state == GLUT_DOWN )
		{
			g_mouse_camera.leftMouseButtonDown = true;

			rprCameraGetInfo(g_camera,RPR_CAMERA_LOOKAT,sizeof(g_mouse_camera.lookat),&g_mouse_camera.lookat,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_UP,sizeof(g_mouse_camera.up),&g_mouse_camera.up,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_POSITION,sizeof(g_mouse_camera.pos),&g_mouse_camera.pos,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_TRANSFORM,sizeof(g_mouse_camera.mat),&g_mouse_camera.mat,0);

			g_mouse_camera.mousePosAtMouseButtonDown_X = x;
			g_mouse_camera.mousePosAtMouseButtonDown_Y = y;

		}
		else if ( state == GLUT_UP )
		{
			g_mouse_camera.leftMouseButtonDown = false;

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
		case 's':
		case 'a':
		case 'q':
		case 'd':
		{

			rprCameraGetInfo(g_camera,RPR_CAMERA_LOOKAT,sizeof(g_mouse_camera.lookat),&g_mouse_camera.lookat,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_UP,sizeof(g_mouse_camera.up),&g_mouse_camera.up,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_TRANSFORM,sizeof(g_mouse_camera.mat),&g_mouse_camera.mat,0);
			rprCameraGetInfo(g_camera,RPR_CAMERA_POSITION,sizeof(g_mouse_camera.pos),&g_mouse_camera.pos,0);

			RadeonProRender::float3 lookAtVec = g_mouse_camera.lookat - g_mouse_camera.pos;
			lookAtVec.normalize();

			RadeonProRender::float3 vecRight = RadeonProRender::cross(g_mouse_camera.up,lookAtVec);
			vecRight.normalize();

			cameraMoves = true;

			const float speed = 0.5f;

			switch (key)
			{

				case 'w':
				case 'z': // for azerty keyboard
				{
					g_mouse_camera.pos += lookAtVec * speed;
					break;
				}

				case 's':
				{
					g_mouse_camera.pos -= lookAtVec * speed;
					break;
				}

				case 'a':
				case 'q': // for azerty keyboard
				{
					g_mouse_camera.pos += vecRight * speed;
					break;
				}

				case 'd':
				{
					g_mouse_camera.pos -= vecRight * speed;
					break;
				}

		
			}

			break;
		}


		case 'x':
		{
			g_askExit = true;
			break;
		}

		default:
			break;
	}

	if ( cameraMoves )
	{
		g_mouse_camera.mat.m30 = g_mouse_camera.pos.x;
		g_mouse_camera.mat.m31 = g_mouse_camera.pos.y;
		g_mouse_camera.mat.m32 = g_mouse_camera.pos.z;

		rprCameraSetTransform( g_camera, false, &g_mouse_camera.mat.m00 );

		// camera moved, so we need to redraw the framebuffer.
		CHECK( rprFrameBufferClear(g_frame_buffer) );
	}

}

void Display()
{
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
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_FLOAT, static_cast<const GLvoid*>(g_fbdata));         

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
	CHECK(rprObjectDelete(g_camera));g_camera=nullptr;
	CHECK(rprObjectDelete(g_matsys));g_matsys=nullptr;
	CHECK(rprObjectDelete(g_frame_buffer));g_frame_buffer=nullptr;
	CHECK(rprObjectDelete(g_frame_buffer_2));g_frame_buffer_2=nullptr;
	g_gc.GCClean();
	CHECK(rprObjectDelete(g_scene));g_scene=nullptr;
	CheckNoLeak(g_context);
	CHECK(rprObjectDelete(g_context));g_context=nullptr; // Always delete the RPR Context in last.
}

int main(int argc, char** argv)
{
	//	enable RPR API trace
	//	set this before any RPR API calls
	//	rprContextSetParameter1u(0,RPR_CONTEXT_TRACING_ENABLED,1);

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

	std::cout << "RPR SDK simple rendering tutorial.\n";

	rpr_int status = RPR_SUCCESS;

	// Register the plugin.
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

	// Create an environment light
	CHECK( CreateNatureEnvLight(g_context, g_scene, g_gc, 0.9f) );


	// Create camera
	{
		CHECK( rprContextCreateCamera(g_context, &g_camera) );

		// Position camera in world space: 
		CHECK( rprCameraLookAt(g_camera, 0.0f, 5.0f, 20.0f,    0, 1, 0,   0, 1, 0) );

		// set camera field of view
		CHECK( rprCameraSetFocalLength(g_camera, 75.f) );

		// Set camera for the scene
		CHECK( rprSceneSetCamera(g_scene, g_camera) );
	}
	// Set scene to render for the context
	CHECK( rprContextSetScene(g_context, g_scene) );


	// create a teapot shape
	rpr_shape teapot01 = nullptr;
	{
		teapot01 = ImportOBJ("../../Resources/Meshes/teapot.obj",g_scene,g_context);
		g_gc.GCAdd(teapot01);

		RadeonProRender::matrix m0 = RadeonProRender::rotation_x(MY_PI);
		CHECK(rprShapeSetTransform(teapot01, RPR_TRUE, &m0.m00));
	}

	// create the floor
	CHECK( CreateAMDFloor(g_context, g_scene, g_matsys, g_gc, 1.0f, 1.0f)  );

	// Create material for the teapot
	{
		rpr_image uberMat2_img1 = nullptr;
		CHECK(rprContextCreateImageFromFile(g_context,"../../Resources/Textures/lead_rusted_Base_Color.jpg",&uberMat2_img1));
		g_gc.GCAdd(uberMat2_img1);

		rpr_image uberMat2_img2 = nullptr;
		CHECK(rprContextCreateImageFromFile(g_context,"../../Resources/Textures/lead_rusted_Normal.jpg",&uberMat2_img2));
		g_gc.GCAdd(uberMat2_img2);
	
		rpr_material_node uberMat2_imgTexture1 = nullptr;
		CHECK(rprMaterialSystemCreateNode(g_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture1));
		g_gc.GCAdd(uberMat2_imgTexture1);
		CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture1,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img1));

		rpr_material_node uberMat2_imgTexture2 = nullptr;
		CHECK(rprMaterialSystemCreateNode(g_matsys,RPR_MATERIAL_NODE_IMAGE_TEXTURE,&uberMat2_imgTexture2));
		g_gc.GCAdd(uberMat2_imgTexture2);
		CHECK(rprMaterialNodeSetInputImageDataByKey(uberMat2_imgTexture2,   RPR_MATERIAL_INPUT_DATA  ,uberMat2_img2));

		rpr_material_node matNormalMap = nullptr;
		CHECK( rprMaterialSystemCreateNode(g_matsys,RPR_MATERIAL_NODE_NORMAL_MAP,&matNormalMap));
		g_gc.GCAdd(matNormalMap);
		CHECK( rprMaterialNodeSetInputFByKey(matNormalMap,RPR_MATERIAL_INPUT_SCALE,1.0f,1.0f,1.0f,1.0f));
		CHECK( rprMaterialNodeSetInputNByKey(matNormalMap,RPR_MATERIAL_INPUT_COLOR,uberMat2_imgTexture2));

		rpr_material_node uberMat2 = nullptr;
		CHECK(rprMaterialSystemCreateNode(g_matsys,RPR_MATERIAL_NODE_UBERV2,&uberMat2));
		g_gc.GCAdd(uberMat2);

		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_COLOR   ,uberMat2_imgTexture1));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_NORMAL   ,matNormalMap));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_DIFFUSE_WEIGHT    ,1, 1, 1, 1));

		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_COLOR  ,uberMat2_imgTexture1));
		CHECK(rprMaterialNodeSetInputNByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_NORMAL   ,matNormalMap));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_WEIGHT  ,1, 1, 1, 1));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ROUGHNESS     ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY    ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_ANISOTROPY_ROTATION  ,0, 0, 0, 0));
		CHECK(rprMaterialNodeSetInputUByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_MODE   ,RPR_UBER_MATERIAL_IOR_MODE_METALNESS));
		CHECK(rprMaterialNodeSetInputFByKey(uberMat2, RPR_MATERIAL_INPUT_UBER_REFLECTION_IOR   ,1.36, 1.36, 1.36, 1.36));

		CHECK(rprShapeSetMaterial(teapot01, uberMat2));
	}

	CHECK( rprContextSetParameterByKey1f(g_context, RPR_CONTEXT_DISPLAY_GAMMA , 2.2f ) ); // set display gamma

	// Create framebuffer to store rendering result
	rpr_framebuffer_desc desc = { WINDOW_WIDTH,WINDOW_HEIGHT };

	// 4 component 32-bit float value each
	rpr_framebuffer_format fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
	CHECK(rprContextCreateFrameBuffer(g_context, fmt, &desc, &g_frame_buffer));
	CHECK(rprContextCreateFrameBuffer(g_context, fmt, &desc, &g_frame_buffer_2));

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));

	// This line can be added for faster RPR rendering.
	// The higher RPR_CONTEXT_PREVIEW, the faster RPR rendering, ( but more pixelated ).
	// 0 = disabled (defaut value)
	//CHECK( rprContextSetParameterByKey1u(g_context,RPR_CONTEXT_PREVIEW, 1u ) );

	// Set framebuffer for the context
	CHECK(rprContextSetAOV(g_context, RPR_AOV_COLOR, g_frame_buffer));

	// Define the update callback.
	// During the rprContextRender execution, RPR will call it regularly
	// The 'CALLBACK_DATA' : 'g_update' is not used by RPR. it can be any data structure that the API user wants.
	CHECK(rprContextSetParameterByKeyPtr(g_context, RPR_CONTEXT_RENDER_UPDATE_CALLBACK_FUNC, (void*)GuiRenderImpl::notifyUpdate));
	CHECK(rprContextSetParameterByKeyPtr(g_context, RPR_CONTEXT_RENDER_UPDATE_CALLBACK_DATA, &g_update));


	// do a first rendering iteration, just for to force scene/cache building.
	std::cout << "Cache and scene building... ";
	CHECK(rprContextSetParameterByKey1u(g_context,RPR_CONTEXT_ITERATIONS,1));
	CHECK( rprContextRender( g_context ) );
	std::cout << "done\n";

	// each rprContextRender call will do `g_batchSize` iterations. 
	// Note that calling rprContextRender 1 time with RPR_CONTEXT_ITERATIONS = `g_batchSize` is faster than calling rprContextRender `g_batchSize` times with RPR_CONTEXT_ITERATIONS = 1
	CHECK(rprContextSetParameterByKey1u(g_context,RPR_CONTEXT_ITERATIONS,g_batchSize));

	// allocate the data that will be used the read RPR framebuffer, and give it to OpenGL.
	g_fbdata = std::shared_ptr<float>(new float[WINDOW_WIDTH * WINDOW_HEIGHT * 4], std::default_delete<float[]>());

	std::cout << "Press W or S to move the camera.\n";

	glutDisplayFunc(Display);
	glutIdleFunc(Update);
	glutKeyboardFunc(OnKeyboardEvent);
	glutMouseFunc(OnMouseEvent);
	glutMotionFunc(OnMouseMoveEvent);
	glutMainLoop();


	// this should be called when the OpenGL Application closes
	// ( note that it may not be called, as GLUT doesn't have "Exit" callback - press X key in order to have a cleaned exit )
	OnExit();


	return 0;
}


