#include "ShaderProgram.h"
#include "../Utils.h"

#include <iostream>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

namespace ShaderInfo
{
	const char* getTypeString(GLenum type) {
	    // There are many more types than are covered here, but
	    // these are the most common in these examples.
	    switch (type) {
	        case GL_FLOAT:
	            return "float";
	        case GL_FLOAT_VEC2:
	            return "vec2";
	        case GL_FLOAT_VEC3:
	            return "vec3";
	        case GL_FLOAT_VEC4:
	            return "vec4";
	        case GL_DOUBLE:
	            return "double";
	        case GL_INT:
	            return "int";
	        case GL_UNSIGNED_INT:
	            return "unsigned int";
	        case GL_BOOL:
	            return "bool";
	        case GL_FLOAT_MAT2:
	            return "mat2";
	        case GL_FLOAT_MAT3:
	            return "mat3";
	        case GL_FLOAT_MAT4:
	            return "mat4";
	        default:
	            return "?";
	    }
	}
}

ShaderProgram::ShaderProgram() : ID{0}, linked{GL_FALSE} {}

ShaderProgram::~ShaderProgram()
{
	if (ID == 0) return;

	DetachAndDeleteShaderObjects();
	glDeleteProgram(ID);
}

void ShaderProgram::CompileShader(std::string fileName, ShaderType type)
{
	std::string path = SHADER_PATH + fileName;
	auto shaderCode = Utils::readFile(path);
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
	}
	else
	{
		LoadUniformLocations();
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

void ShaderProgram::Use()
{
	if (ID <= 0 || (!linked))
	{
		throw ShaderProgramException("Shader has not been linked");
	}

	glUseProgram(ID);
}

void ShaderProgram::BindAttributeLocation(GLuint location, const char* name)
{
	glBindAttribLocation(ID, location, name);
}

#pragma region Uniforms

template<typename T>
void ShaderProgram::SetUniform(const char* name, T value)
{
	static_assert(Utils::falseTemplate<T>);
}

template<>
void ShaderProgram::SetUniform<glm::vec3>(const char* name, glm::vec3 val)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, val.x, val.y, val.z);	
}

template<>
void ShaderProgram::SetUniform<glm::mat4>(const char* name, glm::mat4 val)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));	
}

template<>
void ShaderProgram::SetUniform<GLfloat>(const char* name, GLfloat val)
{
	GLint loc = GetUniformLocation(name);
    glUniform1f(loc, val);
}

template<typename T>
void ShaderProgram::SetUniform(const char* name, T v1, T v2, T v3)
{
	static_assert(Utils::falseTemplate<T>);
}

template<>
void ShaderProgram::SetUniform<GLfloat>(const char* name, GLfloat x, GLfloat y, GLfloat z)
{
	GLint loc = GetUniformLocation(name);
    glUniform3f(loc, x, y, z);
}

GLint ShaderProgram::GetUniformLocation(const char* name)
{
	if (uniformLocations.find(name) == uniformLocations.end()) 
	{
		throw std::runtime_error("Uniform location invalid!");
	}

	return uniformLocations[name];
}

void ShaderProgram::LoadUniformLocations()
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

		/*if (glGetUniformBlockIndex(ID, name) == GL_INVALID_INDEX)
			continue; // Skip uniforms in blocks*/

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

		/*if (glGetUniformBlockIndex(ID, name) == GL_INVALID_INDEX)
			continue; // Skip uniforms in blocks*/

		std::cout << " - " << glGetUniformLocation(ID, name) << " " << name << " " << ShaderInfo::getTypeString(type) << std::endl;
	}
}

#pragma endregion

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

#pragma region Getters

GLint ShaderProgram::GetID()
{
	return ID;
}

GLboolean ShaderProgram::IsLinked()
{
	return linked;
}

#pragma endregion 