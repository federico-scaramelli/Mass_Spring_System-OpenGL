#pragma once

#include <stdexcept>
#include <unordered_map>

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
protected:
	GLint ID;
	GLboolean linked;
	std::unordered_map<std::string, GLint> uniformLocations;
	GLint GetUniformLocation(const char* name);
	
public:
	ShaderProgram();
	~ShaderProgram();

	void Setup();

	void CompileShader (std::string fileName, ShaderType type);

	void Use() const;
	void DetachAndDeleteShaderObjects();
	void Link();
	void Validate();

	GLint GetID();
	GLboolean IsLinked();

	void BindAttributeLocation(GLuint location, const char* name);

	template<typename T>
	void SetUniform(const char* name, T value);
	template<typename T>
	void SetUniformArray(const char* name, T value);
	
	void LoadUniformLocations();
	void PrintActiveUniforms();
};