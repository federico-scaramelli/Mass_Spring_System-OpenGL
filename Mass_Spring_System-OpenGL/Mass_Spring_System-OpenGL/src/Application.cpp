#pragma region Includes

#include <iostream>

#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/Renderer.h"

#include "engine/Camera.h"
#include "mass-spring/Wind.h"
#include "mass-spring/Cloth.h"
#include "mass-spring/Rope.h"
#include "engine/Scene.h"
#include "engine/LightSource.h"
#include "engine/CollidingSphere.h"

#include "glm/gtc/type_ptr.inl"
#include "mass-spring/ClothPresets.h"
#include "mass-spring/PhysicsParameters.h"

#pragma endregion

Window window{};
GLFWwindow* glfwWindow = nullptr;


void init ()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run ()
{
#pragma region Scene Creation
	//Camera
	auto* camera = new Camera (window.GetAspectRatio());
	Scene::GetInstance()->AddCamera (camera);

	// LIGHT
	auto* lightSource = new LightSource();
	Scene::GetInstance()->AddGameObject (lightSource);
	lightSource->GetTransform().AddPosition ({ 122.5, -80, 122.5 });
	dynamic_cast<SolidMaterialUI*>(lightSource->GetUI().m_MaterialUI)->SetValues({1, 1, 0.6f});

	// WIND
	auto* wind = new Wind (10, 50, 200);
	Scene::GetInstance()->AddGameObject (wind);
	wind->GetTransform().AddPosition ({ -100, -100, 0 });
	wind->GetTransform().AddRotation ({ 0, -90, 0 });
	dynamic_cast<SolidMaterialUI*>(wind->GetUI().m_MaterialUI)->SetValues({0, 0.32, 1});

	// CLOTH
	auto* cloth = new Cloth ("Curtain", &ClothPresets::curtain);
	cloth->PinTopEdge();
	Scene::GetInstance()->AddGameObject (cloth);
	cloth->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// FLAG
	auto* flag = new Cloth ("Flag", &ClothPresets::flag);
	flag->PinLeftBorderVertices();
	Scene::GetInstance()->AddGameObject (flag);
	flag->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// TRAMPOLINE
	auto* trampoline = new Cloth ("Trampoline", &ClothPresets::trampoline);
	trampoline->PinAllEdges();
	Scene::GetInstance()->AddGameObject (trampoline);
	trampoline->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// FLAG
	auto* towel = new Cloth ("Towel", &ClothPresets::towel);
	towel->PinCenter();
	Scene::GetInstance()->AddGameObject (towel);
	towel->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// ROPE
	/*auto* rope = new Rope(10, 10, 2);
	Scene::GetInstance()->AddGameObject(rope);*/

	// CUBE
	// Primitive cube("Cube", CUBE, 300);
	//scene.AddGameObject (&cube);

#pragma endregion

	Physics::lastTime = glfwGetTime();
	while (!glfwWindowShouldClose (glfwWindow))
	{
		Physics::nowTime = glfwGetTime();
		Physics::deltaTime += (Physics::nowTime - Physics::lastTime)
			/ Physics::fixedDeltaTime;
		Physics::lastTime = Physics::nowTime;

		Renderer::GetInstance()->Clear();

		Scene::GetInstance()->Update();

		Renderer::GetInstance()->DrawUI();

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
