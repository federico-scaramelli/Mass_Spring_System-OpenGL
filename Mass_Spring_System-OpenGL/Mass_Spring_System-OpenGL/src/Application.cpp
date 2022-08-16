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
#include "mass-spring/Cloth.h"
#include "mass-spring/Rope.h"
#include "engine/Scene.h"
#include "engine/LightSource.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.inl"
#include "imgui/imgui.h"

#pragma endregion

Window window{};
GLFWwindow* glfwWindow = nullptr;
Renderer renderer;
Scene scene{ &renderer };

void init() {
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run() {

#pragma region Scene Creation
	//Camera
	Camera camera(window.GetAspectRatio());
	scene.AddCamera(&camera);

	//Layout
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat>(4); //0
	vertexBufferLayout.Push<GLfloat>(4); //1
	vertexBufferLayout.Push<GLfloat>(4); //2
	vertexBufferLayout.Push<GLfloat>(4); //3
	vertexBufferLayout.Push<GLfloat>(4); //4
	vertexBufferLayout.Push<GLfloat>(4); //5

	// CLOTH
	Cloth cloth(5.f, 5.f, 30, 30);

	cloth.GetMaterial().CreateShaderProgram({ {"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT} });

	// Compute stage 1: compute new positions without constraints
	
	cloth.firstStageComputeShader.CreateProgram ( {"clothShader.comp", ShaderType::COMPUTE} );
	cloth.firstStageComputeShader.SetWorkGroupSize({ 10, 10, 1 });
	cloth.firstStageComputeShader.SetWorkGroupNum( { cloth.GetClothSize(), 1 } );

	// Compute stage 2: apply constraints
	cloth.secondStageComputeShader.CreateProgram ( {"clothConstraints.comp", ShaderType::COMPUTE} );
	cloth.secondStageComputeShader.SetWorkGroupSize({ 10, 10, 1 });
	cloth.secondStageComputeShader.SetWorkGroupNum( { cloth.GetClothSize(), 1 } );

	cloth.GetMesh().SetBuffers(vertexBufferLayout);
	cloth.SetComputeBuffers();

	scene.AddGameObject(&cloth);

	// ROPE
	//TODO: binding deve essere diverso sennò sovrascrive le robe del cloth -> serve un altro shader per la rope
	Rope rope(50, 10, 1);
	rope.GetMesh().SetBuffers(vertexBufferLayout);
	rope.GetMaterial().CreateShaderProgram({ {"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT} });
	scene.AddGameObject(&rope);

	// LIGHT
	LightSource lightSource{};
	lightSource.GetMesh().SetBuffers(vertexBufferLayout);
	lightSource.GetMesh().GetMaterial().CreateShaderProgram({ {"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT} });
	scene.AddLightSource(&lightSource);

#pragma endregion

#pragma region UI Elements Creation

	renderer.AddBoolCheckboxUI("Wireframe", &renderer.wireframe);
	renderer.AddBoolCheckboxUI("Backface", &renderer.backface);

#pragma endregion

	while (!glfwWindowShouldClose(glfwWindow))
	{
		renderer.Clear();

		scene.Update();

		renderer.DrawUI();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}
}

int main() {
	try
	{
		init();
		run();
	}
	catch (std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
