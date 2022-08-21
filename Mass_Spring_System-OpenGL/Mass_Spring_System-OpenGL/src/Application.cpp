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

	// LIGHT
	LightSource lightSource{};
	scene.AddLightSource (&lightSource);
	lightSource.GetTransform().AddPosition ({ 122.5, -80, 122.5 });

	// WIND
	Wind wind{10, 50, 50};
	scene.AddWind (&wind);

	// CLOTH
	int size = 50;
	float linkLenght = 10;
	Cloth cloth (size, size, linkLenght); 
	cloth.PinCenter();
	scene.AddGameObject (&cloth);
	cloth.GetTransform().AddPosition ({ -(size * linkLenght / 2), 0, -(size * linkLenght / 2) });

	// SPHERE
	CollidingSphere sphere ("Sphere", 100);
	scene.AddGameObject (&sphere);
	sphere.GetTransform().AddPosition ({ 0, -200, 0 });

	// SPHERE
	CollidingSphere sphere2 ("Sphere2", 50);
	scene.AddGameObject (&sphere2);
	sphere2.GetTransform().AddPosition ({ -150, -100, 0 });

	// SPHERE
	CollidingSphere sphere3 ("Sphere3", 50);
	scene.AddGameObject (&sphere3);
	sphere3.GetTransform().AddPosition ({ 150, -100, 0 });

	// CUBE
	Primitive cube ("Cube", CUBE, 300);
	//scene.AddGameObject (&cube);

	// ROPE
	Rope rope (50, 5, 10);
	scene.AddGameObject (&rope);

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
