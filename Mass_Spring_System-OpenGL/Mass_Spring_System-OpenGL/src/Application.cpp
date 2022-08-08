#include <iostream>
#include "engine/Window.h"
#include "engine/GraphicsShader.h"
#include "engine/ShaderProgram.h"

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
		-0.5f, -0.5f, 0.0f,	  //0
		0.5f, -0.5f, 0.0f,	  //1
		0.5f, 0.5f, 0.0f,	  //2
		-0.5f, 0.5f, 0.0f,	  //3
	};

	GLuint vertexPosIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLfloat vertexColors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.3f, 0.6f, 0.8f,
	};

	VertexArray vertexArrayObject;
	VertexBufferFormat floatVec3{GL_FLOAT, 3, GL_FALSE};

	VertexBuffer vbPositions(vertexPositions, sizeof(vertexPositions));
	vbPositions.SetFormat<GLfloat>(3);

	IndexBuffer indexBuffer(vertexPosIndices, sizeof(vertexPosIndices));

	VertexBuffer vbColors(vertexColors, sizeof(vertexColors));
	vbColors.SetFormat<GLfloat>(3);

	vertexArrayObject.AddBuffer(vbColors);
	vertexArrayObject.AddBuffer(vbPositions);

	ShaderProgram basicShader{};
	basicShader.CompileShader("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();
	basicShader.Use();

	
	GLfloat angle = 0;
	while (!glfwWindowShouldClose(glfwWindow)) {
		//Clear the color buffer
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rot matrix used in uniform
		angle += 0.01f;
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,0,1));
		basicShader.SetUniform<glm::mat4>("rotationMatrix", rotationMatrix);

		vertexArrayObject.Bind();
		indexBuffer.Bind();

		//Draw the bound buffers data
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, sizeof(vertexPosIndices) / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);

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
