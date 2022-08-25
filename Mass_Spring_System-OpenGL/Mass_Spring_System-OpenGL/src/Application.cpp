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
#include "mass-spring/PhysicsParameters.h"

#pragma endregion

Window window{};
GLFWwindow* glfwWindow = nullptr;


void init()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run()
{
#pragma region Scene Creation
	//Camera
	auto* camera = new Camera(window.GetAspectRatio());
	Scene::GetInstance()->AddCamera(camera);

	// LIGHT
	auto* lightSource = new LightSource();
	Scene::GetInstance()->AddGameObject(lightSource);
	lightSource->GetTransform().AddPosition({122.5, -80, 122.5});

	// WIND
	auto* wind = new Wind(10, 50, 50);
	Scene::GetInstance()->AddGameObject(wind);
	wind->GetTransform().AddPosition ( {-100, -100, 0} );
	wind->GetTransform().AddRotation(  {0, -90, 0} );

	// ROPE
	auto* rope = new Rope(10, 10, 2);
	Scene::GetInstance()->AddGameObject(rope);

	// CLOTH
	int size = 50;
	auto* cloth = new Cloth("Cloth", size, size, 10);
	cloth->PinAllEdges();
	//cloth->PinCenter();
	Scene::GetInstance()->AddGameObject(cloth);
	cloth->GetTransform().SetRotation({90, 0, 0});
	cloth->GetUI().m_TransformUI->SetPositionRange({-700, 700});

	// FLAG
	auto* flag = new Cloth("Flag", size, size * 0.7 , 10);
	flag->PinLeftBorderVertices();
	Scene::GetInstance()->AddGameObject(flag);
	flag->GetTransform().AddPosition({-(size * 10 / 2), 0, 0});
	flag->GetUI().m_TransformUI->SetPositionRange({-700, 700});
	flag->GetTransform().AddPosition ( {300, -100, 0} );

	// CUBE
	// Primitive cube("Cube", CUBE, 300);
	//scene.AddGameObject (&cube);

#pragma endregion

	Physics::lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(glfwWindow))
	{
		Physics::nowTime = glfwGetTime();
		Physics::deltaTime += (Physics::nowTime - Physics::lastTime)
								/ Physics::fixedDeltaTime;
		Physics::lastTime = Physics::nowTime;

		Renderer::GetInstance()->Clear();

		Scene::GetInstance()->Update();

		Renderer::GetInstance()->DrawUI();

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
