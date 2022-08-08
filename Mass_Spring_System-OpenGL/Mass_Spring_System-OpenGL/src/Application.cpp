#include <iostream>
#include "engine/Window.h"
#include "engine/GraphicsShader.h"

#include "DebugLogger.h"
#include "engine/Renderer.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"


Window window{};
GLFWwindow* glfwWindow = nullptr;

void init()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
	
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

	VertexArray vertexArrayObject;
	VertexBufferFormat floatVec3{GL_FLOAT, 3, GL_FALSE};

	VertexBuffer vbPositions(vertexPositions, sizeof(vertexPositions));
	vbPositions.SetFormat<GLfloat>(3);

	VertexBuffer vbColors(vertexColors, sizeof(vertexColors));
	vbColors.SetFormat<GLfloat>(3);

	vertexArrayObject.AddBuffer(vbPositions);
	vertexArrayObject.AddBuffer(vbColors);

	GraphicsShader basicShader("res/shaders/shader.vert", "res/shaders/shader.frag");
	
	basicShader.Use();
	
	while (!glfwWindowShouldClose(glfwWindow)) {
		GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
		//Clear the color buffer
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		//Rot matrix used in uniform
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 180.f, glm::vec3(0,0,1));
		
		//Location of uniform
		GLint location = glGetUniformLocation(basicShader.ID, "rotationMatrix");

		if( location < 0 )
		{
			throw std::runtime_error("Uniform location invalid!");
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
		vertexArrayObject.Bind();

		//Draw the bound buffers data
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

		glfwSwapBuffers(glfwWindow);

		glfwPollEvents();
	}
}

int main()
{
	try {
		init();
		run();
	} catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
