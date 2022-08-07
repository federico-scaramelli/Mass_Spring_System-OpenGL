#include "GraphicsShader.h"

GraphicsShader::GraphicsShader(const std::string vertexPath, const std::string fragmentPath)
{
	//Vertex shader
	auto vertexShaderCode = utils::readFile(vertexPath);
	const GLchar* vertexShaderSource[] = { vertexShaderCode.c_str() };
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Fragment shader
	auto fragmentShaderCode = utils::readFile(fragmentPath);
	const GLchar* fragmentShaderSource[] = { fragmentShaderCode.c_str() };
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GraphicsShader::~GraphicsShader()
{
	glDeleteProgram(ID);
}

void GraphicsShader::Use()
{
	glUseProgram(ID);
}
