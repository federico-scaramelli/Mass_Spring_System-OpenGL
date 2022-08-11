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
#include "Cloth.h"
#include "Rope.h"
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

void init()
{
	glfwWindow = window.GetGLFWWindow();
	EnableDebug();
}

void run()
{
#pragma region Scene Creation

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(2);

	//ROPE
	Rope rope(50, 1000, 1);
	rope.GetMesh().SetBuffers (vertexBufferLayout);

	//CLOTH
	Cloth cloth(50.f, 50.f, 100, 100);
	cloth.SetColor ({1.f, 0.f, 0.f});
	cloth.GetMesh().SetBuffers (vertexBufferLayout);

	//LIGHT
	LightSource lightSource { {0.8f, 0.8f, 0.8f} };
	lightSource.GetMesh().SetBuffers (vertexBufferLayout);

#pragma endregion

#pragma region BasicShader Creation

	ShaderProgram basicShader{};
	basicShader.CompileShader("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();
	basicShader.Use();

#pragma endregion
	
	basicShader.SetUniform<glm::mat4>("projectionMatrix", camera.GetProjectionMatrix());

#pragma region UI Elements Creation

	float cameraPosition[3] = {0, 0, 100.f};
	float cameraRotation[3] = {0, 0, 0};
	renderer.AddFloatSliderUI("Camera Position", cameraPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Camera 'Rotation'", cameraRotation, -180.f, 180.f);

	float objectPosition[3] = {0, 0, 0};
	float objectRotation[3] = {0, 0, 0};
	renderer.AddFloatSliderUI("Object Position", objectPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI("Object Rotation", objectRotation, -180.f, 180.f);

	float lightPosition[3] = {0, 0, 0};
	float lightIntensity[3] = {0, 0, 0};
	renderer.AddFloatSliderUI ("Light Position", lightPosition, -100.f, 100.f);
	renderer.AddFloatSliderUI ("Light Intensity", lightIntensity, 0.f, 1.f);

	const char* sceneOptions[] {"Cloth", "Rope"};
	int selectedScene = 0;
	renderer.AddListBoxUI("To render objects", &selectedScene, sceneOptions, 2);

	renderer.AddBoolCheckboxUI("Wireframe", &renderer.wireframe);
	renderer.AddBoolCheckboxUI("Backface", &renderer.backface);

#pragma endregion

	while (!glfwWindowShouldClose(glfwWindow)) {
		renderer.Clear();

		camera.GetTransform().SetRotation({cameraRotation[0], cameraRotation[1], cameraRotation[2]});
		camera.GetTransform().SetPosition({cameraPosition[0], cameraPosition[1], cameraPosition[2]});

		glm::mat4 viewMatrix = camera.GetUpdatedViewMatrix();
		glm::mat4 modelViewMatrix{};

		//LIGHT
		lightSource.GetTransform().SetPosition ({lightPosition[0], lightPosition[1], lightPosition[2]});
		basicShader.SetUniform<glm::vec3>("lightPosition", lightSource.GetTransform().GetPosition());
		basicShader.SetUniform<glm::vec3>("lightAmbient", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<glm::vec3>("lightDiffuse", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<glm::vec3>("lightSpecular", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<glm::vec3>("matAmbient", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<glm::vec3>("matDiffuse", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<glm::vec3>("matSpecular", glm::vec3( lightIntensity[0], lightIntensity[1], lightIntensity[2] ));
		basicShader.SetUniform<GLfloat>("matShininess", 2);
		modelViewMatrix = viewMatrix * lightSource.GetTransform().GetUpdatedModelMatrix();
		basicShader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
		basicShader.SetUniform<glm::mat3>("normalMatrix", glm::inverseTranspose( glm::mat3( modelViewMatrix )));
		renderer.Draw (lightSource.GetMesh().vertexArrayObject, rope.GetMesh().indexBuffer, basicShader);

		switch (selectedScene)
		{
		case 0:
			cloth.GetTransform().SetPosition({objectPosition[0], objectPosition[1], objectPosition[2]});
			cloth.GetTransform().SetRotation({objectRotation[0], objectRotation[1], objectRotation[2]});
			modelViewMatrix = viewMatrix * cloth.GetTransform().GetUpdatedModelMatrix();
			basicShader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
			basicShader.SetUniform<glm::mat3>("normalMatrix", glm::inverseTranspose( glm::mat3( modelViewMatrix )));
			renderer.Draw(cloth.GetMesh().vertexArrayObject, cloth.GetMesh().indexBuffer, basicShader);
			break;

		case 1:
			rope.GetTransform().SetPosition({objectPosition[0], objectPosition[1], objectPosition[2]});
			rope.GetTransform().SetRotation({objectRotation[0], objectRotation[1], objectRotation[2]});
			modelViewMatrix = viewMatrix * rope.GetTransform().GetUpdatedModelMatrix();
			basicShader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
			basicShader.SetUniform<glm::mat3>("normalMatrix", glm::inverseTranspose( glm::mat3( modelViewMatrix )));
			renderer.Draw(rope.GetMesh().vertexArrayObject, rope.GetMesh().indexBuffer, basicShader);
			break;

		default:
			std::cout << "No scene setup avaiable!";
		}

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
