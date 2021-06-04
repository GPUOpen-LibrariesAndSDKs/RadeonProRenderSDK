/*****************************************************************************\
*
*  Module Name    ShaderManager.h
*  Project        Radeon ProRender SDK rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials
*
*  Copyright(C) 2020-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#ifndef BVHOQ_shader_manager_h
#define BVHOQ_shader_manager_h

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#elif defined(WIN32)
#define NOMINMAX
#include <Windows.h>
#include "GL/glew.h"
#include "GLUT/GLUT.h"
#elif defined(__LINUX__)
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <string>
#include <map>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	GLuint GetProgram(std::string const& progName);

private:
	GLuint CompileProgram(std::string const& progName);

	ShaderManager(ShaderManager const&);
	ShaderManager& operator = (ShaderManager const&);

	std::map<std::string, GLuint> shaderCache_;
};


#endif
