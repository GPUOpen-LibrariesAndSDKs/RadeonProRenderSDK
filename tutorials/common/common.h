/*****************************************************************************\
*
*  Module Name    common.h
*  Project        Radeon ProRender SDK rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#pragma once

#include <assert.h> 
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "RadeonProRender.h"

// Tahoe plugin is in maintenance mode - no new features planned.
#if 0
#if defined(WIN32)
#define RPR_PLUGIN_FILE_NAME "Tahoe64.dll"
#elif defined(__LINUX__)
#define RPR_PLUGIN_FILE_NAME "libTahoe64.so"
#elif defined(__APPLE__)
#define RPR_PLUGIN_FILE_NAME "libTahoe64.dylib"
#endif

// Northstar is the successor of Tahoe. It's in active development since 2020
#else
#if defined(WIN32)
#define RPR_PLUGIN_FILE_NAME "Northstar64.dll"
#elif defined(__LINUX__)
#define RPR_PLUGIN_FILE_NAME "libNorthstar64.so"
#elif defined(__APPLE__)
#define RPR_PLUGIN_FILE_NAME "libNorthstar64.dylib"
#endif
#define USING_NORTHSTAR
#endif


// creation flags used by most of the Demo.
// examples:
// RPR_CREATION_FLAGS_ENABLE_GPU0 <- use the first GPU
// RPR_CREATION_FLAGS_ENABLE_CPU <- use the first CPU only
// RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_CPU <- use the first CPU+GPU
// RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_GPU1 <-- use 2 GPU
const rpr_creation_flags g_ContextCreationFlags = RPR_CREATION_FLAGS_ENABLE_GPU0
	#if defined(USING_NORTHSTAR) && defined(__APPLE__)
	| RPR_CREATION_FLAGS_ENABLE_METAL // by default always enable Metal for MacOS
	#endif
	;


// Structure to describe vertex layout
struct vertex
{
	rpr_float pos[3];
	rpr_float norm[3];
	rpr_float tex[2];
};

// Cube geometry
extern vertex cube_data[];

// Plane geometry
extern vertex plane_data[];

// Cube indices
extern rpr_int indices[];

// Number of vertices per face
extern rpr_int num_face_vertices[];


#define CHECK(x) { if ( (x) != RPR_SUCCESS ) { ErrorManager(x,__FILE__,__LINE__); } }
#define CHECK_EQ(x,y) { if ( (x) != (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_NE(x,y) { if ( (x) == (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_GT(x,y) { if ( (x) <= (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_GE(x,y) { if ( (x) <  (y) ) { ErrorManager(0,__FILE__,__LINE__); } }

void ErrorManager(int errorCode, const char* fileName, int line, rpr_context ctx=nullptr);

// Number of iterations for rendering
int const NUM_ITERATIONS = 200;


// this is an example a good practice to check RPR memory leak
void CheckNoLeak(rpr_context context);


// create a rpr_shape from OBJ file
// This is a very basic importer: doesn't support of the features from the spec.
// This function is only developed and tested for simple meshes, like the matball scene, the teapot.
// For a better OBJ importer, check the project 64_mesh_obj_demo in this SDK
rpr_shape ImportOBJ(const std::string& file, rpr_scene scene, rpr_context ctx);


// Create a scene with  or several matballs. Used by several demos.
class MatballScene
{
public:

	MatballScene();

	struct MATBALL
	{
		MATBALL() 
		{  
			base=nullptr;
			inner=nullptr;
			outer=nullptr;
		}
		rpr_shape base;
		rpr_shape inner;
		rpr_shape outer;

		void SetMaterial(rpr_material_node mat)
		{
			rprShapeSetMaterial(base,mat);
			rprShapeSetMaterial(inner,mat);
			rprShapeSetMaterial(outer,mat);
		}
	};

	MATBALL Init(rpr_context context, int shapeShiftX, int shapeShiftY, bool forceUberMaterialForFloor=false);

	void Clean();

	void Render(const std::string& outImgFileName, int iterationCount = 100, bool useResolveFramebuffer=true);

	MATBALL AddMatball(int shiftX, int shiftY, bool createAsInstance=true);

	void CameraLook1Shape(int shiftX, int shiftY);
	void CameraLook9Shape();


	rpr_scene m_scene; 
	rpr_material_system m_matsys;
	rpr_camera m_camera;
	rpr_context m_context;
	rpr_shape m_shape_floor;
	rpr_material_node m_floorMaterial;
	rpr_image m_floorImage;
	rpr_image m_IBLimage;
	rpr_light m_light;
	rpr_material_node m_floorImageMaterial;
	rpr_framebuffer m_frame_buffer;
	rpr_framebuffer m_frame_buffer_res;

	std::vector<MATBALL> m_matballs;

	const float m_matballGap = 0.6f;

	const rpr_framebuffer_desc m_framebuffer_desc = { 640 , 480};
	const rpr_framebuffer_format m_framebuffer_fmt = { 4, RPR_COMPONENT_TYPE_FLOAT32 };
};


//
// Simple Garbage Collector for Radeon ProRender objects.
// Add RPR objects with GCAdd. Then call GCClean to clear each object
//
class RPRGarbageCollector
{
public:

	void GCAdd(rpr_material_node n) { m_rprNodesCollector.push_back(n); }
	void GCAdd(rpr_image n)			{ m_rprNodesCollector.push_back(n); }
	void GCAdd(rpr_shape n)			{ m_rprNodesCollector.push_back(n); }
	void GCAdd(rpr_light n)			{ m_rprNodesCollector.push_back(n); }
	void GCAdd(rpr_framebuffer n)	{ m_rprNodesCollector.push_back(n); }
	void GCAdd(rpr_camera n)		{ m_rprNodesCollector.push_back(n); }
	
	void GCClean()
	{
		for(const auto& i : m_rprNodesCollector)
			if ( i ) { CHECK(rprObjectDelete(i));  }
		m_rprNodesCollector.clear();
	}

private:

	std::vector<void*> m_rprNodesCollector;
};


// Create a simple plane with RPR logo as texture. commonly used by several demos.
rpr_status CreateAMDFloor(
	rpr_context context, 
	rpr_scene scene, 
	rpr_material_system matsys, 
	RPRGarbageCollector& gc, 
	float scaleX, float scaleY,
	float translationX=0.0f, float translationY=0.0f, float translationZ=0.0f
	);

// Create an environment light from an HDR image. commonly used by several demos.
rpr_status CreateNatureEnvLight(rpr_context context, rpr_scene scene, RPRGarbageCollector& gc, float power);

// some helper functions to create quads meshes easily
rpr_shape CreateQuad(RPRGarbageCollector& gc, rpr_context context, rpr_scene scene, vertex* meshVertices, unsigned int meshVertices_nbOfElement );
rpr_shape CreateQuad_YZ(RPRGarbageCollector& gc, rpr_context context, rpr_scene scene, float ax, float ay, float bx, float by, float X, float normal);
rpr_shape CreateQuad_XZ(RPRGarbageCollector& gc, rpr_context context, rpr_scene scene, float ax, float ay, float bx, float by, float Y, float normal);
rpr_shape CreateQuad_XY(RPRGarbageCollector& gc, rpr_context context, rpr_scene scene, float ax, float ay, float bx, float by, float Z, float normal);

