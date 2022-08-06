#pragma once
#include <string>
#include "Utils.h"
#include "glad/glad.h"

class GraphicsShader
{
public:
	GLuint ID;

	GraphicsShader(const std::string vertexPath, const std::string fragmentPath);

	~GraphicsShader();

	void Use();
};
