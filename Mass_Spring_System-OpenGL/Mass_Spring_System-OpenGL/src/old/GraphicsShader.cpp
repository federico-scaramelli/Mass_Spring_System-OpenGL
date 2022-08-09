#include "GraphicsShader.h"

#include <iostream>

#include "Renderer.h"

GraphicsShader::GraphicsShader(const std::string vertexPath, const std::string fragmentPath)
{
	//Vertex shader
	auto vertexShaderCode = Utils::readFile(vertexPath);
	const GLchar* vertexShaderSource[] = { vertexShaderCode.c_str() };
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertexShader, 1, vertexShaderSource, NULL));
	GLCall(glCompileShader(vertexShader));

	//Fragment shader
	auto fragmentShaderCode = Utils::readFile(fragmentPath);
	const GLchar* fragmentShaderSource[] = { fragmentShaderCode.c_str() };
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Create program from shaders
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	GLint uniformsCount = 0;
	GLCall(glGetProgramInterfaceiv(ID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount));

	GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

	std::cout << "Active uniforms" << std::endl;

	for (int i=0; i<uniformsCount; ++i)
	{
		GLint results[4];

		glGetProgramResourceiv(ID, GL_UNIFORM, i, 4, properties, 4, NULL, results);

		if(results[3] != -1)
		{
			continue; //Skips uniforms in blocks
		}

		GLint nameBufSize = results[0]+1;
		char* name = new char[nameBufSize];

		glGetProgramResourceName(ID, GL_UNIFORM, i, nameBufSize, NULL, name);

		std::cout << results[2] << " " << name << " " << std::endl;
		
	}

}

GraphicsShader::~GraphicsShader()
{
	glDeleteProgram(ID);
}

void GraphicsShader::Use()
{
	glUseProgram(ID);
}
