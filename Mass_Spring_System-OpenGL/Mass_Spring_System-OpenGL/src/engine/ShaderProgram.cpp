#include "ShaderProgram.h"

#include <iostream>

#include "../Utils.h"

ShaderProgram::ShaderProgram() : ID{0}, linked{GL_FALSE}
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(ID);
}

void ShaderProgram::CompileShader(std::string fileName, ShaderType type)
{
	std::string path = SHADER_PATH + fileName;
	auto shaderCode = utils::readFile(path);
	const GLchar* shaderSource[] = {shaderCode.c_str()};
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, shaderSource, NULL);
	glCompileShader(shader);

	if (ID == 0)
	{
		ID = glCreateProgram();

		if (ID == 0)
		{
			throw ShaderProgramException("Unable to create shader program.");
		}
	}

	glAttachShader(ID, shader);
}

void ShaderProgram::Use()
{
	if (ID <= 0 || (!linked))
	{
		throw ShaderProgramException("Shader has not been linked");
	}

	glUseProgram(ID);
}

void ShaderProgram::DetachAndDeleteShaderObjects()
{
	GLint numShaders = 0;
	glGetProgramiv(ID, GL_ATTACHED_SHADERS, &numShaders);
	std::vector<GLuint> shaderNames(numShaders);
	glGetAttachedShaders(ID, numShaders, NULL, shaderNames.data());
	for (GLuint shader : shaderNames)
	{
		glDetachShader(ID, shader);
		glDeleteShader(shader);
	}
}

void ShaderProgram::Link()
{
	if (linked) return;

	if (ID <= 0)
	{
		throw ShaderProgramException("Program has not been compiled.");
	}

	glLinkProgram(ID);

	GLint status;
	GLchar message[1024];

	glGetProgramiv(ID, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		GLsizei log_length = 0;

		glGetProgramInfoLog(ID, 1024, &log_length, message);
		// throw ShaderProgramException(message);
	}
	else
	{
		FindUniformLocations();
		linked = true;
	}


	DetachAndDeleteShaderObjects();

	if (GL_FALSE == status)
		throw ShaderProgramException(message);
}

void ShaderProgram::Validate()
{
	if (!IsLinked())
		throw ShaderProgramException("Program is not linked");

	GLint status;
	glValidateProgram(ID);
	glGetProgramiv(ID, GL_VALIDATE_STATUS, &status);

	if (GL_FALSE == status)
	{
		// Store log and return false
		int length = 0;
		std::string logString;

		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			char* c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(ID, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		throw ShaderProgramException(std::string("Program failed to validate\n") + logString);
	}
}

GLint ShaderProgram::GetID()
{
	return ID;
}

GLboolean ShaderProgram::IsLinked()
{
	return linked;
}

void ShaderProgram::BindAttributeLocation(GLuint location, const char* name)
{
	glBindAttribLocation(ID, location, name);
}

void ShaderProgram::FindUniformLocations()
{
	uniformLocations.clear();

	GLint numUniforms = 0;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);

	GLchar name[256];
	GLsizei length;
	GLint size;
	GLenum type;
	for (GLint i = 0; i < numUniforms; ++i)
	{
		glGetActiveUniform(ID, i, sizeof(name), &length, &size, &type, name);

		if (glGetUniformBlockIndex(ID, name) == GL_INVALID_INDEX)
			continue; // Skip uniforms in blocks

		uniformLocations[name] = glGetUniformLocation(ID, name);
	}
}

void ShaderProgram::PrintActiveUniforms()
{
	std::cout << "Active uniforms" << std::endl;

	GLint numUniforms = 0;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);

	GLchar name[256];
	GLsizei length;
	GLint size;
	GLenum type;
	for (GLint i = 0; i < numUniforms; ++i)
	{
		glGetActiveUniform(ID, i, sizeof(name), &length, &size, &type, name);

		if (glGetUniformBlockIndex(ID, name) == GL_INVALID_INDEX)
			continue; // Skip uniforms in blocks

		std::cout << " - " << glGetUniformLocation(ID, name) << " " << name << " " << ShaderInfo::getTypeString(type) << std::endl;
	}
}
