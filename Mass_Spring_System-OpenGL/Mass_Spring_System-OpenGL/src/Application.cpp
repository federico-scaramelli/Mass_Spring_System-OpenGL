#include <iostream>

#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"
#include "engine/VertexBufferLayout.h"

//#include "DebugLogger.h"
#include "VertexStructures.h"
#include "engine/Renderer.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"


Window window{};
GLFWwindow* glfwWindow = nullptr;
Renderer renderer;

void init ()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run ()
{
	std::array<Vertex, 4> quadVertices{
		{
			{ { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f } }
		}
	};

	GLuint vertexPosIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray vertexArrayObject;

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (3);


	GLsizei sizeOfDatasInBytes = quadVertices.size() * sizeof (Vertex);
	VertexBuffer vertexBuffer{ quadVertices.data(), sizeOfDatasInBytes };

	vertexArrayObject.AddBuffer (vertexBuffer, vertexBufferLayout);

	IndexBuffer indexBuffer (vertexPosIndices, sizeof(vertexPosIndices));

	ShaderProgram basicShader{};
	basicShader.CompileShader ("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader ("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();
	basicShader.Use();

	GLfloat angle = 0;
	while (!glfwWindowShouldClose (glfwWindow)) {
		renderer.Clear();

		//Rot matrix used in uniform
		angle += 0.01f;
		glm::mat4 rotationMatrix = glm::rotate (glm::mat4 (1.0f), angle, glm::vec3 (0, 0, 1));
		basicShader.SetUniform<glm::mat4> ("rotationMatrix", rotationMatrix);

		//Draw the bound buffers data
		renderer.Draw (vertexArrayObject, indexBuffer, basicShader);

		glfwSwapBuffers (glfwWindow);
		glfwPollEvents();
	}
}

int main ()
{
	try {
		init();
		run();
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
