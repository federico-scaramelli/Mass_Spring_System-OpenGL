#pragma region Includes

#include <iostream>

#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"
#include "engine/VertexBufferLayout.h"
#include "engine/Renderer.h"

#include "engine/Vertex.h"
#include "engine/Camera.h"
#include "Cloth.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

#pragma endregion

Window window{};
GLFWwindow* glfwWindow = nullptr;
Renderer renderer;
Camera camera (window.GetAspectRatio());

void init ()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run ()
{
#pragma region Cloth Creation

	Cloth cloth (50.f, 50.f, 100, 100);

	VertexArray vertexArrayObject;

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (2);

	std::vector<Vertex>& vertices = cloth.GetVertices();
	GLsizei sizeOfVertices = vertices.size() * sizeof(Vertex);
	VertexBuffer vertexBuffer{vertices.data(), sizeOfVertices};

	vertexArrayObject.AddVertexBuffer (vertexBuffer, vertexBufferLayout);

	std::vector<GLuint>& indices = cloth.GetIndices();
	auto sizeOfIndices=indices.size() * sizeof(GLuint);
	IndexBuffer indexBuffer(indices.data(), sizeOfIndices);

#pragma endregion

#pragma region BasicShader Creation

	ShaderProgram basicShader{};
	basicShader.CompileShader ("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader ("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();
	basicShader.Use();

#pragma endregion

	camera.GetTransform().SetPosition ({ 0.f, 0.f, -10.f });
	basicShader.SetUniform<glm::mat4> ("projectionMatrix", camera.GetProjectionMatrix());

#pragma region UI Elements Creation

	float cameraPosition[3] = { 0, 0, -10.f };
	float cameraRotation[3] = { 0, 0, 0 };
	renderer.AddFloatSliderUI("Camera Position", cameraPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Camera 'Rotation'", cameraRotation, -180.f, 180.f);

	renderer.AddBoolCheckboxUI ("Wireframe", &renderer.wireframe);

#pragma endregion

	while (!glfwWindowShouldClose (glfwWindow)) 
	{
		renderer.Clear();

		renderer.DrawUI();

		camera.GetTransform().SetRotation ({ cameraRotation[0], cameraRotation[1], cameraRotation[2] });
		camera.GetTransform().SetPosition ({ cameraPosition[0], cameraPosition[1], cameraPosition[2] });

		basicShader.SetUniform<glm::mat4> ("viewMatrix", camera.GetUpdatedViewMatrix());
		basicShader.SetUniform<glm::mat4> ("modelMatrix", glm::mat4{ 1.f });
		
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