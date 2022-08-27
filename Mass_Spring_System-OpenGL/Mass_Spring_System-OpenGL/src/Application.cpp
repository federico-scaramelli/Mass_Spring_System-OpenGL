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
	lightSource->GetTransform().AddPosition ({ 0, 100, -50 });
	if (lightSource->GetMaterial().fragShader == Solid)
	  dynamic_cast<SolidMaterialUI*> (lightSource->GetUI().m_MaterialUI)->SetValues ({ 1, 1, 0.6f });

	// WIND
	auto* wind = new Wind (10, 50, 200);
	Scene::GetInstance()->AddGameObject (wind);
	wind->GetTransform().AddPosition ({ -100, -100, 0 });
	wind->GetTransform().AddRotation ({ 0, -90, 0 });
	if (wind->GetMaterial().fragShader == Solid)
	  dynamic_cast<SolidMaterialUI*> (wind->GetUI().m_MaterialUI)->SetValues ({ 0, 0.32, 1 });

	// ROPE
	auto* rope = new Rope (256, 1, 1.0f,
	                       {
								 0.032f, 16, 1.0f,
								 { 0.f, -500, 0.f, 0.f },
								 1.0f, 1000.0f, 1.0f, 1.0f,
								 0.25f, 1.1f,
								 0.85f, 1.05f, 0.98f
	                         }
	);
	Scene::GetInstance()->AddGameObject (rope);

	// CLOTH
	auto* curtain = new Cloth ("Curtain", &ClothPresets::curtain);
	curtain->PinTopEdge();
	Scene::GetInstance()->AddGameObject (curtain);
	curtain->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });

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

	// TOWEL
	auto* towel = new Cloth ("Towel", &ClothPresets::towel);
	towel->PinCenter();
	Scene::GetInstance()->AddGameObject (towel);
	towel->GetUI().m_TransformUI->SetPositionRange ({ -700, 700 });
	
	

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
