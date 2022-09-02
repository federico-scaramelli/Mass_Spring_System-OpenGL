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
	// Objects instantiation and placement into the scene
#pragma region Scene Creation
	// Camera
	auto* camera = new Camera (window.GetAspectRatio());
	Scene::GetInstance()->AddCamera (camera);

	// Light
	auto* lightSource = new LightSource();
	Scene::GetInstance()->AddGameObject (lightSource);
	lightSource->GetTransform().AddPosition ({ 122.5, -80, 122.5 });
	dynamic_cast<SolidMaterialUI*> (lightSource->GetUI().m_MaterialUI)->SetValues ({ 1, 1, 0.6f });

	// Wind
	auto* wind = new Wind (10, 50, 200);
	Scene::GetInstance()->AddGameObject (wind);
	wind->GetTransform().AddPosition ({ -100, -100, 0 });
	wind->GetTransform().AddRotation ({ 0, -90, 0 });
	dynamic_cast<SolidMaterialUI*> (wind->GetUI().m_MaterialUI)->SetValues ({ 0, 0.32, 1 });

	// Rope - All presets
	MassSpringParameters ropeParam = {
										   0.032f, 16, 1.0f,
										   { 0.f, -500, 0.f, 0.f },
										   1.0f, 1000.0f, 1.0f, 1.0f,
										   0.25f, 1.1f,
										   0.85f, 1.05f, 0.98f
										};
	auto* ropeFirst			= new Rope ("RopeFirst", 256, 2, 1.0f, ropeParam);
	ropeFirst->PinFirstNode();
	auto* ropeEdges			= new Rope ("RopeEdges", 256, 2, 1.0f, ropeParam);
	ropeEdges->PinEdgesNode();
	auto* ropeMiddle			= new Rope ("RopeMiddle", 256, 2, 1.0f, ropeParam);
	ropeMiddle->PinMiddleNode();
	auto* ropeMiddleAndEdges	= new Rope ("RopeMiddleAndEdges", 256, 1, 1.0f, ropeParam);
	ropeMiddleAndEdges->PinMiddleAndEdgesNode();
	auto* ropeQuarters			= new Rope ("RopeQuarters", 256, 2, 1.0f, ropeParam);
	ropeQuarters->PinQuarters();
	auto* ropeTenths			= new Rope ("RopeTenths", 256, 2, 1.0f, ropeParam);
	ropeTenths->PinTenths();

	Scene::GetInstance()->AddGameObject (ropeFirst);
	Scene::GetInstance()->AddGameObject (ropeEdges);
	Scene::GetInstance()->AddGameObject (ropeMiddle);
	Scene::GetInstance()->AddGameObject (ropeMiddleAndEdges);
	Scene::GetInstance()->AddGameObject (ropeQuarters);
	Scene::GetInstance()->AddGameObject (ropeTenths);

	// Cloth - Curtain
	auto* cloth = new Cloth ("Curtain", &ClothPresets::curtain);
	cloth->PinTopEdge();
	Scene::GetInstance()->AddGameObject (cloth);
	cloth->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// Cloth - Flag
	auto* flag = new Cloth ("Flag", &ClothPresets::flag);
	flag->PinLeftBorderVertices();
	Scene::GetInstance()->AddGameObject (flag);
	flag->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// Cloth - Trampoline
	auto* trampoline = new Cloth ("Trampoline", &ClothPresets::trampoline);
	trampoline->PinAllEdges();
	Scene::GetInstance()->AddGameObject (trampoline);
	trampoline->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

	// Cloth - Towel
	auto* towel = new Cloth ("Towel", &ClothPresets::towel);
	towel->PinCenter();
	Scene::GetInstance()->AddGameObject (towel);
	towel->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

#pragma endregion

	Physics::lastTime = glfwGetTime();

	// Main loop
	while (!glfwWindowShouldClose (glfwWindow))
	{
		// Physics fixed delta time
		Physics::nowTime = glfwGetTime();
		Physics::deltaTime += (Physics::nowTime - Physics::lastTime)
			/ Physics::fixedDeltaTime;
		Physics::lastTime = Physics::nowTime;

		// Update and rendering
		Renderer::GetInstance()->Clear();
		Scene::GetInstance()->Update();
		Renderer::GetInstance()->DrawUI();

		glfwSwapBuffers (glfwWindow);
		glfwPollEvents();
	}
}

// Entry point
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
