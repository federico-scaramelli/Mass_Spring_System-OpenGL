#include <iostream>
#include "engine/Window.hpp"
#include "engine/GraphicsShader.h"

#include "engine/Renderer.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"

GLFWwindow* window = nullptr;
GLuint vaoHandle;
GLuint vboHandles[2];

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
	/*glDeleteVertexArrays(1, &vaoHandle);
	glDeleteBuffers(1, &vboHandles[0]);
	glDeleteBuffers(1, &vboHandles[1]);*/

	glfwDestroyWindow(window);
	glfwTerminate();
}

void run()
{
	GLfloat vertexPositions[] = {
	  -0.8f, -0.8f, 0.0f,
	  0.8f, -0.8f, 0.0f,
	  0.0f, 0.8f, 0.0f
	};

	GLfloat vertexColors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	VertexArray va;
	VertexBufferLayout layout;

	VertexBuffer vbPositions(vertexPositions, sizeof(vertexPositions));
	layout.Push<GLfloat>(3);
	va.AddBuffer(vbPositions, layout);

	VertexBuffer vbColors(vertexColors, sizeof(vertexColors));
	layout.Push<GLfloat>(3);
	va.AddBuffer(vbColors, layout);

	GraphicsShader basicShader("res/shaders/shader.vert", "res/shaders/shader.frag");


	/*//Create and populate the buffer objects
	glGenBuffers(2, vboHandles);
	GLuint verticesBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	//Populate the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW); 

	//Populate the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

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
	glVertexAttribBinding(1, 1);*/

	
	while(!glfwWindowShouldClose(window)) 
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		basicShader.Use();

		//glBindVertexArray(vaoHandle);
		va.Bind();

		//Draw the bound buffers data
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
