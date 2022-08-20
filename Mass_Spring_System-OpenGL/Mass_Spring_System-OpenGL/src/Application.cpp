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
#include "engine/CollidingSphere.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.inl"
#include "imgui/imgui.h"
#include "mass-spring/PhysicsSolver.h"

#pragma endregion

Window window{};
GLFWwindow* glfwWindow = nullptr;
Renderer renderer;
PhysicsSolver physicsSolver;
Scene scene{ &renderer, &physicsSolver };


void init ()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run ()
{
#pragma region Scene Creation
	//Camera
	Camera camera (window.GetAspectRatio());
	scene.AddCamera (&camera);

	//Layout
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat> (4); //0
	vertexBufferLayout.Push<GLfloat> (4); //1
	vertexBufferLayout.Push<GLfloat> (4); //2
	vertexBufferLayout.Push<GLfloat> (4); //3
	vertexBufferLayout.Push<GLfloat> (4); //4
	vertexBufferLayout.Push<GLfloat> (4); //5

	// CLOTH
	int size = 50;
	float linkLenght = 10;
	Cloth cloth (size, size, linkLenght);

	//cloth.PinCenter();

	cloth.GetMesh().SetBuffers (vertexBufferLayout);
	cloth.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});

	// Compute stage 1: compute new positions without constraints
	cloth.simulationStageComputeShader.CreateProgram ({ "eulerClothShader.comp", ShaderType::COMPUTE });
	cloth.simulationStageComputeShader.SetWorkGroupSize ({ 16, 16, 1 });
	cloth.simulationStageComputeShader.SetWorkGroupNum ({ cloth.GetClothSize(), 1 });

	// Compute stage 2: apply constraints
	cloth.constraintsStageComputeShader.CreateProgram ({ "clothConstraints.comp", ShaderType::COMPUTE });
	cloth.constraintsStageComputeShader.SetWorkGroupSize ({ 16, 16, 1 });
	cloth.constraintsStageComputeShader.SetWorkGroupNum ({ cloth.GetClothSize(), 1 });

	cloth.SetComputeBuffers();
	scene.AddGameObject (&cloth);
	cloth.GetTransform().AddPosition ({ -(size * linkLenght / 2), 0, -(size * linkLenght / 2) });


	// SPHERE
	CollidingSphere sphere ("Sphere", 100);
	sphere.GetMesh().SetBuffers (vertexBufferLayout);
	sphere.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	scene.AddGameObject (&sphere);
	sphere.GetTransform().AddPosition ({ 0, -200, 0 });

	// SPHERE
	CollidingSphere sphere2 ("Sphere2", 50);
	sphere2.GetMesh().SetBuffers (vertexBufferLayout);
	sphere2.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	scene.AddGameObject (&sphere2);
	sphere2.GetTransform().AddPosition ({ -150, -100, 0 });

	// SPHERE
	CollidingSphere sphere3 ("Sphere3", 50);
	sphere3.GetMesh().SetBuffers (vertexBufferLayout);
	sphere3.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	scene.AddGameObject (&sphere3);
	sphere3.GetTransform().AddPosition ({ 150, -100, 0 });

	// CUBE
	Primitive cube ("Cube", CUBE, 300);
	cube.GetMesh().SetBuffers (vertexBufferLayout);
	cube.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	//scene.AddGameObject (&cube);


	// ROPE
	Rope rope (50, 5, 10);
	rope.GetMesh().SetBuffers (vertexBufferLayout);
	rope.GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	scene.AddGameObject (&rope);

	// LIGHT
	LightSource lightSource{};
	lightSource.GetMesh().SetBuffers (vertexBufferLayout);
	lightSource.GetMesh().GetMaterial().CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX }, { "blinnPhongShader.frag", ShaderType::FRAGMENT }
	});
	scene.AddLightSource (&lightSource);

	lightSource.GetTransform().AddPosition ({ 122.5, -80, 122.5 });

#pragma endregion

	physicsSolver.lastTime = glfwGetTime();
	while (!glfwWindowShouldClose (glfwWindow))
	{
		physicsSolver.nowTime = glfwGetTime();
		PhysicsSolver::deltaTime += (physicsSolver.nowTime - physicsSolver.lastTime)
			/ physicsSolver.fixedDeltaTime;
		physicsSolver.lastTime = physicsSolver.nowTime;

		renderer.Clear();

		scene.Update();

		renderer.DrawUI();

		glfwSwapBuffers (glfwWindow);
		glfwPollEvents();
	}
}

int main ()
{
	try
	{
		init();
		run();
	}
	catch (std::runtime_error& e) { std::cout << e.what() << std::endl; }

	return 0;
}
