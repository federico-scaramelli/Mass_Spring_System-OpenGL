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
Scene scene{&renderer};

void init()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run()
{

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

	// CLOTH
	Cloth cloth(5.f, 5.f, 10, 10);
	cloth.GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	cloth.GetMaterial().CreateComputeShaderProgram({{"shader.comp", ShaderType::COMPUTE}});

	cloth.GetComputeShader().SetWorkGroupSize({10,1,1});
	cloth.GetComputeShader().SetWorkGroupNumberFromVertexNumber(cloth.GetMesh().GetVertices().size());

	cloth.GetMesh().SetBuffers(vertexBufferLayout);
	cloth.GetMesh().SetComputeBuffers();

	

	scene.AddGameObject(&cloth);

	// ROPE
	//TODO: binding deve essere diverso sennò sovrascrive le robe del cloth -> serve un altro shader per la rope
	Rope rope(50, 10, 1);
	rope.GetMesh().SetBuffers(vertexBufferLayout);
	rope.GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	scene.AddGameObject(&rope);

	// LIGHT
	LightSource lightSource{};
	lightSource.GetMesh().SetBuffers(vertexBufferLayout);
	lightSource.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
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

int main()
{
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
