#include <iostream>
#include "Window.hpp"

GLFWwindow* window = nullptr;
GLuint vaoHandle;
GLuint vboHandles[2];
GLuint shaderProgram;

bool init()
{
	window = CreateWindow();
	if(window == nullptr) {
		return false;
	}
	return true;
}

void cleanup()
{
	glDeleteVertexArrays(1, &vaoHandle);
	glDeleteBuffers(1, &vboHandles[0]);
	glDeleteBuffers(1, &vboHandles[1]);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void run()
{
	GLfloat vertices[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f
	};

	GLfloat vertexColors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	//Vertex shader
	auto vertexShaderCode = utils::readFile("shaders/shader.vert");
	const GLchar* vertexShaderSource[] = { vertexShaderCode.c_str() };
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Fragment shader
	auto fragmentShaderCode = utils::readFile("shaders/shader.frag");
	const GLchar* fragmentShaderSource[] = { fragmentShaderCode.c_str() };
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Create and populate the buffer objects
	glGenBuffers(2, vboHandles);
	GLuint verticesBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	//Populate the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	//Populate the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

	//Create and setup the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//Enable the vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Map index 0 to the position buffer
	glBindVertexBuffer(0, verticesBufferHandle, 0, sizeof(GLfloat) * 3);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	// Map index 1 to the color buffer
	glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);
	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(1, 1);

	
	while(!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	cleanup();
}

int main()
{
	if (init()) 
	{
		run();
	}
	
	return 0;
}
