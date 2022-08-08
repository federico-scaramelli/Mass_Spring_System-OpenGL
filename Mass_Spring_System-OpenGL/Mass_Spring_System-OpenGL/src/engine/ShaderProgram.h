#pragma once

#include <map>
#include <stdexcept>

#include "glad/glad.h"

#ifndef SHADER_PATH
  #ifdef NDEBUG
	  #define SHADER_PATH "../res/shaders/"
	#else
	  #define SHADER_PATH "res/shaders/" 
  #endif
#endif

enum ShaderType {
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	TESS_CONTROL = GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};

class ShaderProgramException : public std::runtime_error
{
public:
	ShaderProgramException( const std::string & msg ) : std::runtime_error(msg){}
};

class ShaderProgram {
private:
	GLint ID;
	GLboolean linked;
	std::map<std::string, GLint> uniformLocations;
	GLint GetUniformLocation(const char* name);


public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	void CompileShader (std::string fileName, ShaderType type);

	void Use();
	void DetachAndDeleteShaderObjects();
	void Link();
	void Validate();

	GLint GetID();
	GLboolean IsLinked();

	void BindAttributeLocation(GLuint location, const char* name);
	// void BindFragDataLocation(GLuint location, const char* name);

	template<typename T>
	void SetUniform(const char* name, T... value);

	void FindUniformLocations();
	void PrintActiveUniforms();



};

namespace ShaderInfo
{
	const char *getTypeString(GLenum type) {
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