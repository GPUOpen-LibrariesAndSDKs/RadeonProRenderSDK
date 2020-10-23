#pragma once

#include <assert.h> 
#include <iostream>

// creation flags used by most of the Demo.
// examples:
// RPR_CREATION_FLAGS_ENABLE_GPU0 <- use the first GPU
// RPR_CREATION_FLAGS_ENABLE_CPU <- use the first CPU only
// RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_CPU <- use the first CPU+GPU
// RPR_CREATION_FLAGS_ENABLE_GPU0 | RPR_CREATION_FLAGS_ENABLE_GPU1 <-- use 2 GPU
rpr_creation_flags g_ContextCreationFlags = RPR_CREATION_FLAGS_ENABLE_GPU0;


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
g_ContextCreationFlags |= RPR_CREATION_FLAGS_ENABLE_METAL; // by default always enable Metal for MacOS
#endif
#define USING_NORTHSTAR
#endif


// Structure to describe vertex layout
struct vertex
{
	rpr_float pos[3];
	rpr_float norm[3];
	rpr_float tex[2];
};

// Cube geometry
vertex cube_data[] = 
{
	{ -1.0f, 1.0f, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
	{  1.0f, 1.0f, -1.0f, 0.f, 1.f, 0.f, 0.f, 0.f },
	{  1.0f, 1.0f, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f },
	{  -1.0f, 1.0f, 1.0f , 0.f, 1.f, 0.f, 0.f, 0.f},

	{  -1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
	{  1.0f, -1.0f, -1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
	{  1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },
	{  -1.0f, -1.0f, 1.0f , 0.f, -1.f, 0.f, 0.f, 0.f },

	{  -1.0f, -1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
	{  -1.0f, -1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
	{  -1.0f, 1.0f, -1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },
	{  -1.0f, 1.0f, 1.0f , -1.f, 0.f, 0.f, 0.f, 0.f },

	{  1.0f, -1.0f, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
	{  1.0f, -1.0f, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
	{  1.0f, 1.0f, -1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },
	{  1.0f, 1.0f, 1.0f ,  1.f, 0.f, 0.f, 0.f, 0.f },

	{  -1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
	{  1.0f, -1.0f, -1.0f ,  0.f, 0.f, -1.f , 0.f, 0.f },
	{  1.0f, 1.0f, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },
	{  -1.0f, 1.0f, -1.0f ,  0.f, 0.f, -1.f, 0.f, 0.f },

	{  -1.0f, -1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
	{  1.0f, -1.0f, 1.0f , 0.f, 0.f,  1.f, 0.f, 0.f },
	{  1.0f, 1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
	{  -1.0f, 1.0f, 1.0f , 0.f, 0.f, 1.f, 0.f, 0.f },
};

// Plane geometry
vertex plane_data[] = 
{
	{-15.f, 0.f, -15.f, 0.f, 1.f, 0.f, 0.f, 0.f},
	{-15.f, 0.f,  15.f, 0.f, 1.f, 0.f, 0.f, 1.f},
	{ 15.f, 0.f,  15.f, 0.f, 1.f, 0.f, 1.f, 1.f},
	{ 15.f, 0.f, -15.f, 0.f, 1.f, 0.f, 1.f, 0.f},
};

// Cube indices
rpr_int indices[] = 
{
	3,1,0,
	2,1,3,

	6,4,5,
	7,4,6,

	11,9,8,
	10,9,11,

	14,12,13,
	15,12,14,

	19,17,16,
	18,17,19,

	22,20,21,
	23,20,22
};

// Number of vertices per face
rpr_int num_face_vertices[] = 
{
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};


#define CHECK(x) { if ( (x) != RPR_SUCCESS ) { ErrorManager(x,__FILE__,__LINE__); } }
#define CHECK_EQ(x,y) { if ( (x) != (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_NE(x,y) { if ( (x) == (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_GT(x,y) { if ( (x) <= (y) ) { ErrorManager(0,__FILE__,__LINE__); } }
#define CHECK_GE(x,y) { if ( (x) <  (y) ) { ErrorManager(0,__FILE__,__LINE__); } }

void ErrorManager(int errorCode, const char* fileName, int line)
{
	std::cout<<"ERROR detected - program will stop."<<std::endl;
	std::cout<<"file = "<< fileName << std::endl;
	std::cout<<"line = "<< line << std::endl;
	std::cout<<"error code = "<< errorCode << std::endl;
	assert(0);
}

// Number of iterations for rendering
int const NUM_ITERATIONS = 64;


// this is an example a good practice to check RPR memory leak
void CheckNoLeak(rpr_context context)
{
	rpr_int status = RPR_SUCCESS;

	std::vector<rpr_context_info> type;
	type.push_back(RPR_CONTEXT_LIST_CREATED_CAMERAS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_MATERIALNODES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_LIGHTS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_SHAPES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_POSTEFFECTS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_HETEROVOLUMES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_GRIDS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_BUFFERS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_IMAGES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_FRAMEBUFFERS);
	type.push_back(RPR_CONTEXT_LIST_CREATED_SCENES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_CURVES);
	type.push_back(RPR_CONTEXT_LIST_CREATED_MATERIALSYSTEM);
	type.push_back(RPR_CONTEXT_LIST_CREATED_COMPOSITE);
	type.push_back(RPR_CONTEXT_LIST_CREATED_LUT);

	std::vector<void*> listRemainingObjects;

	for(int iType=0; iType<type.size(); iType++)
	{

		size_t sizeParam = 0;
		status = rprContextGetInfo(context,type[iType],0,0,&sizeParam);CHECK(status);
		
		unsigned int nbObject = sizeParam / sizeof(void*);

		if ( nbObject > 0 )
		{
			std::cout<<"leak of "<< nbObject ;
				 if ( type[iType] == RPR_CONTEXT_LIST_CREATED_CAMERAS ) std::cout<<" cameras\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_MATERIALNODES ) std::cout<<" material nodes\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_LIGHTS ) std::cout<<" lights\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_SHAPES ) std::cout<<" shapes\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_POSTEFFECTS ) std::cout<<" postEffects\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_HETEROVOLUMES ) std::cout<<" heteroVolumes\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_GRIDS ) std::cout<<" grids\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_BUFFERS ) std::cout<<" buffers\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_IMAGES ) std::cout<<" images\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_FRAMEBUFFERS ) std::cout<<" framebuffers\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_SCENES ) std::cout<<" scenes\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_CURVES ) std::cout<<" curves\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_MATERIALSYSTEM ) std::cout<<" materialsystems\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_COMPOSITE ) std::cout<<" composites\n";
			else if ( type[iType] == RPR_CONTEXT_LIST_CREATED_LUT ) std::cout<<" luts\n";
			else 
			{
				std::cout<<" ???\n"; 
			}

			unsigned int idFirstTime = listRemainingObjects.size();
			listRemainingObjects.assign( idFirstTime + nbObject, nullptr );
			status = rprContextGetInfo(context,type[iType],sizeParam,&listRemainingObjects[idFirstTime],nullptr);CHECK(status);
		}
	}

	if ( listRemainingObjects.size() != 0 )
	{
		std::cout<<"Warning : this context has some leak ("<< listRemainingObjects.size() <<" item(s))\n";
	}

}
