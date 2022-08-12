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

#pragma region Scene Creation
	//Camera
	scene.AddCamera(&camera);

	//Layout
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat>(4);
	vertexBufferLayout.Push<GLfloat>(4);
	vertexBufferLayout.Push<GLfloat>(4);
	vertexBufferLayout.Push<GLfloat>(4);

	// CLOTH
	Cloth cloth(5.f, 5.f, 10, 10);
	cloth.GetMesh().SetBuffers(vertexBufferLayout);
	cloth.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
	scene.AddGameObject(&cloth);

	// ROPE
	Rope rope(50, 1000, 1);
	rope.GetMesh().SetBuffers(vertexBufferLayout);
	rope.GetMesh().GetMaterial().CreateShaderProgram({{"shader.vert", ShaderType::VERTEX}, {"shader.frag", ShaderType::FRAGMENT}});
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


	ShaderProgram computeShader;
	computeShader.CompileShader ("shader.comp", ShaderType::COMPUTE);
	computeShader.Link();
	computeShader.Validate();
	computeShader.Use();

	
	
	
	// std::vector<Vertex> test = {
	// 	{{0,0,0}},
	// 	{{0,0,0}},
	// 	{{0,0,0}}
	// 	};
	// GLuint computeBufSize = sizeof(Vertex) * test.size();
	
	// GLuint buf;
	// glGenBuffers(1, &buf);
	// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, buf);
	// glBufferData (GL_SHADER_STORAGE_BUFFER, computeBufSize, test.data(), GL_STATIC_DRAW);


	auto vertexCount=cloth.GetMesh().GetVertices().size();

	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, cloth.GetMesh().m_vbo.GetID());
	glBufferData (GL_SHADER_STORAGE_BUFFER, cloth.GetMesh().m_vbo.GetSize(), cloth.GetMesh().GetVertices().data(), GL_DYNAMIC_DRAW);



	while (!glfwWindowShouldClose(glfwWindow))
	{
		computeShader.Use();

		
		//start compute
		int workGroupsNumber=10;
		int workSize=vertexCount/workGroupsNumber;
		glDispatchCompute (workSize, 1, 1); //Work group size x y z (abbiamo 1 work group, da 3 thread ciascuno, specificati nello shader)

		//wait the compute end
		glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );

		// glGetNamedBufferSubData(cloth.GetMesh().m_vbo.GetID(), 0, cloth.GetMesh().m_vbo.GetSize(), test.data());

		// glGetNamedBufferSubData(buf, 0, computeBufSize, test.data());
		//
		// for (auto i : test) {
		// 	std::cout << "(" << i.position.x << ", " << i.position.y << ", " << i.position.z << ") ";
		// }

		std::cout << std::endl;

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
