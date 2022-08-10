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
#include "Rope.h"

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
	Rope rope (50, 1000, 1);

	int scene=0;

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (3);
	vertexBufferLayout.Push<GLfloat> (2);

	//ROPE
	std::vector<Vertex>& verticesRope = rope.GetMesh().GetVertices();
	GLsizei verticesSizeRope = verticesRope.size() * sizeof(Vertex);
	VertexBuffer vertexBufferRope{verticesRope.data(), verticesSizeRope};
	
	std::vector<GLuint>& indicesRope = rope.GetMesh().GetIndices();
	auto indicesSizeRope=indicesRope.size() * sizeof(GLuint);
	IndexBuffer indexBufferRope(indicesRope.data(), indicesSizeRope);

	VertexArray vertexArrayObjectRope;
	vertexArrayObjectRope.AddVertexBuffer (vertexBufferRope, vertexBufferLayout);

	//CLOTH
	std::vector<Vertex>& verticesCloth = cloth.GetMesh().GetVertices();
	GLsizei verticesSizeCloth = verticesCloth.size() * sizeof(Vertex);
	VertexBuffer vertexBufferCloth{verticesCloth.data(), verticesSizeCloth};
	
	std::vector<GLuint>& indicesCloth = cloth.GetMesh().GetIndices();
	auto indicesSizeCloth=indicesCloth.size() * sizeof(GLuint);
	IndexBuffer indexBufferCloth(indicesCloth.data(), indicesSizeCloth);
	
	VertexArray vertexArrayObjectCloth;
	vertexArrayObjectCloth.AddVertexBuffer (vertexBufferCloth, vertexBufferLayout);

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

	float clothPosition[3] = { 0, 0, 0 };
	float clothRotation[3] = { 0, 0, 0 };
	float cameraPosition[3] = { 0, 0, 20.f };
	float cameraRotation[3] = { 0, 0, 0 };

	renderer.AddFloatSliderUI("Camera Position", cameraPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Camera 'Rotation'", cameraRotation, -180.f, 180.f);
	renderer.AddFloatSliderUI("Cloth Position", clothPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Cloth Rotation", clothRotation, -180.f, 180.f);

	renderer.AddBoolCheckboxUI ("Wireframe", &renderer.wireframe);
	renderer.AddBoolCheckboxUI ("Backface", &renderer.backface);

#pragma endregion

	while (!glfwWindowShouldClose (glfwWindow)) 
	{
		renderer.Clear();

		cloth.GetTransform().SetPosition({clothPosition[0], clothPosition[1], clothPosition[2]});
		cloth.GetTransform().SetRotation({clothRotation[0], clothRotation[1], clothRotation[2]});

		camera.GetTransform().SetRotation ({ cameraRotation[0], cameraRotation[1], cameraRotation[2] });
		camera.GetTransform().SetPosition ({ cameraPosition[0], cameraPosition[1], cameraPosition[2] });
		
		basicShader.SetUniform<glm::mat4> ("modelMatrix", cloth.GetTransform().GetUpdatedModelMatrix());
		basicShader.SetUniform<glm::mat4> ("viewMatrix", camera.GetUpdatedViewMatrix());

		switch (scene)
		{
		case 0:
			renderer.Draw (vertexArrayObjectCloth, indexBufferCloth, basicShader);
			break;

		case 1:
			renderer.Draw (vertexArrayObjectRope, indexBufferRope, basicShader);
			break;

			default:
				std::cout << "No scene setup on this index!";
		}
		
		renderer.DrawUI();
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