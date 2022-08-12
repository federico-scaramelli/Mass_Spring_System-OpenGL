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
Camera camera(window.GetAspectRatio());
Scene scene{&renderer};

void init()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run()
{
#pragma region BasicShader Creation

	ShaderProgram basicShader{};
	basicShader.CompileShader("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();

	basicShader.Use();
	basicShader.SetUniform<glm::mat4>("projectionMatrix", camera.GetProjectionMatrix());

#pragma endregion

#pragma region Scene Creation
	//Camera
	scene.AddCamera(&camera);

	//Layout
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(2);

	// CLOTH
	Cloth cloth(50.f, 50.f, 100, 100);
	cloth.GetMesh().SetBuffers(vertexBufferLayout);
	cloth.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	scene.AddGameObject(&cloth);

	 // ROPE
	Rope rope(50, 1000, 1);
	rope.GetMesh().SetBuffers(vertexBufferLayout);
	rope.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	scene.AddGameObject(&rope);

	// LIGHT
	LightSource lightSource{{0.8f, 0.8f, 0.8f}};
	lightSource.GetMesh().SetBuffers(vertexBufferLayout);
	lightSource.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	scene.AddLightSource(&lightSource);

#pragma endregion

#pragma region UI Elements Creation

	renderer.AddFloatSliderUI("Camera Position", scene.cameraPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Camera Rotation", scene.cameraRotation, -180.f, 180.f);
	
	renderer.AddFloatSliderUI("Light Position", scene.lightPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Light Color", scene.lightColor, 0.f, 1.f);

	renderer.AddListBoxUI("To render objects", &scene.selectedObject, scene.sceneObjects, 2);

	renderer.AddFloatSliderUI("Object Position", scene.objectPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Object Rotation", scene.objectRotation, -180.f, 180.f);
	
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
