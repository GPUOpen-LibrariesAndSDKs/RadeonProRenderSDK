/*****************************************************************************\
*
*  Module Name    ShaderManager.cpp
*  Project        Radeon ProRender SDK rendering tutorial
*
*  Description    Radeon ProRender SDK tutorials
*
*  Copyright(C) 2020-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "ShaderManager.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>



static void LoadFileContents(std::string const& name, std::vector<char>& contents, bool binary = false)
{
	std::ifstream in(name, std::ios::in | (binary ? std::ios::binary : (std::ios_base::openmode)0));

	if (in && in.is_open() && !in.fail())
	{
		contents.clear();

		std::streamoff beg = in.tellg();

		in.seekg(0, std::ios::end);

		std::streamoff fileSize = in.tellg() - beg;

		in.seekg(0, std::ios::beg);

		contents.resize(static_cast<unsigned>(fileSize));

		in.read(&contents[0], fileSize);
	}
	else
	{
		std::cout << "Error : " << name << " not found.\n";
		exit(-1);
	}

}

static GLuint CompileShader(std::vector<GLchar> const& shader_source, GLenum type)
{
	GLuint shader = glCreateShader(type);

	GLint len = static_cast<GLint>(shader_source.size());
	GLchar const* source_array = &shader_source[0];

	glShaderSource(shader, 1, &source_array, &len);
	glCompileShader(shader);

	GLint result = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		std::vector<char> log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		log.resize(len);

		glGetShaderInfoLog(shader, len, &result, &log[0]);

		glDeleteShader(shader);

		throw std::runtime_error(std::string(log.begin(), log.end()));

		return 0;
	}

	return shader;
}


ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
	for (auto citer = shaderCache_.cbegin(); citer != shaderCache_.cend(); ++citer)
	{
		glDeleteProgram(citer->second);
	}
}

GLuint ShaderManager::CompileProgram(std::string const& progName)
{
	std::string vsName = progName + ".vsh";
	std::string fsName = progName + ".fsh";

	// Need to wrap the shader program here to be exception-safe
	std::vector<GLchar> sourceCode;


	LoadFileContents(vsName, sourceCode);
	GLuint vertexShader = CompileShader(sourceCode, GL_VERTEX_SHADER);

	/// This part is not exception safe:
	/// if the VS compilation succeeded
	/// and PS compilation fails then VS object will leak
	/// fix this by wrapping the shaders into a class
	LoadFileContents(fsName, sourceCode);
	GLuint fragShader = CompileShader(sourceCode, GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	glLinkProgram(program);

	GLint result = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLint length = 0;
		std::vector<char> log;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		log.resize(length);

		glGetProgramInfoLog(program, length, &result, &log[0]);

		glDeleteProgram(program);

		throw std::runtime_error(std::string(log.begin(), log.end()));
	}

	return program;
}

GLuint ShaderManager::GetProgram(std::string const& progName)
{
	auto iter = shaderCache_.find(progName);

	if (iter != shaderCache_.end())
	{
		return iter->second;
	}
	else
	{
		GLuint program = CompileProgram(progName);
		shaderCache_[progName] = program;
		return program;
	}
}