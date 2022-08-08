#include <iostream>
#include "engine/Window.h"
#include "engine/GraphicsShader.h"

#include "engine/Renderer.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"

Window window{};
GLFWwindow* glfwWindow = nullptr;

void init()
{
	glfwWindow = window.GetGLFWWindow();
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
	VertexBufferFormat floatVec3{GL_FLOAT, 3, GL_FALSE};

	VertexBuffer vbPositions(vertexPositions, sizeof(vertexPositions));
	vbPositions.SetFormat<GLfloat>(3);

	VertexBuffer vbColors(vertexColors, sizeof(vertexColors));
	vbColors.SetFormat<GLfloat>(3);

	va.AddBuffer(vbPositions);
	va.AddBuffer(vbColors);

	GraphicsShader basicShader("res/shaders/shader.vert", "res/shaders/shader.frag");

	while (!glfwWindowShouldClose(glfwWindow)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		basicShader.Use();

		va.Bind();

		//Draw the bound buffers data
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
