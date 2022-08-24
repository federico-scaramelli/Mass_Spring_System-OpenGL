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
	Camera camera(window.GetAspectRatio());
	Scene::GetInstance()->AddCamera(&camera);

	// LIGHT
	LightSource lightSource{};
	Scene::GetInstance()->AddGameObject(&lightSource);
	lightSource.GetTransform().AddPosition({122.5, -80, 122.5});

	// WIND
	Wind wind{10, 50, 50};
	Scene::GetInstance()->AddGameObject(&wind);
	wind.GetTransform().AddPosition({-100, -100, 0});
	wind.GetTransform().AddRotation({0, -90, 0});

	// ROPE
	Rope rope(16, 2.56f, 2.56f);
	Scene::GetInstance()->AddGameObject(&rope);
	rope.GetTransform().AddRotation({0, 0, 1});
	rope.GetTransform().AddPosition({0, -75, 600});

	// CLOTH
	int size = 50;
	float linkLenght = 10;
	Cloth cloth("Cloth", size, size, linkLenght);
	// cloth.PinFourBorderPoints();
	// cloth.PinTopPoints();
	cloth.PinCenter();
	Scene::GetInstance()->AddGameObject(&cloth);
	
	cloth.GetTransform().SetRotation({90, 0, 0});
	cloth.GetUI().m_TransformUI->SetPositionRange({-700, 700});


	// FLAG
	Cloth flag("Flag", size, size * 0.7, linkLenght);
	flag.PinLeftBorderVertices();
	Scene::GetInstance()->AddGameObject(&flag);
	flag.GetTransform().AddPosition({-(size * linkLenght / 2), 0, 0});
	flag.GetUI().m_TransformUI->SetPositionRange({-700, 700});
	flag.GetTransform().AddPosition({300, -100, 0});

	// SPHERE
	CollidingSphere sphere("Sphere", 100);
	sphere.m_IsActive = false;
	Scene::GetInstance()->AddGameObject(&sphere);
	sphere.GetTransform().AddPosition({0, -200, 0});

	// SPHERE
	CollidingSphere sphere2("Sphere2", 50);
	sphere2.m_IsActive = false;
	Scene::GetInstance()->AddGameObject(&sphere2);
	sphere2.GetTransform().AddPosition({-150, -100, 0});

	// SPHERE
	CollidingSphere sphere3("Sphere3", 50);
	sphere3.m_IsActive = false;
	Scene::GetInstance()->AddGameObject(&sphere3);
	sphere3.GetTransform().AddPosition({150, -100, 0});

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
