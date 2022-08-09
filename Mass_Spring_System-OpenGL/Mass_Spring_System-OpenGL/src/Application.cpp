#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Window.h"
#include "engine/ShaderProgram.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"
#include "engine/VertexBufferLayout.h"
#include "engine/Renderer.h"

#include "engine/Vertex.h"
#include "engine/Camera.h"
#include "../Cloth.h"

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"

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
	Cloth cloth(10.f, 10.f, 5, 5);

	std::vector<Vertex> vecQuadVertices{
		
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
		
	};

	std::vector<Vertex*>& vertices=cloth.GetVertices();

	
	// GLuint vertexPosIndices[] = {
	// 	0, 1, 2,
	// 	2, 3, 0
	// };

	std::vector<GLuint> vertexIndices{
		0, 1, 2,
		2, 3, 0
	};

	VertexArray vertexArrayObject;

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(3);
	vertexBufferLayout.Push<GLfloat>(2);
	
	
	// GLsizei sizeOfVertices = vertices.size() * sizeof(Vertex);
	// VertexBuffer vertexBuffer{vertices.data(), sizeOfVertices};

	GLsizei sizeOfVerticesVector = vecQuadVertices.size() * sizeof(Vertex);
	VertexBuffer vertexBuffer{vecQuadVertices.data(), sizeOfVerticesVector};

	vertexArrayObject.AddVertexBuffer(vertexBuffer, vertexBufferLayout);

	auto sizeOfIndices=vertexIndices.size() * sizeof(GLuint);
	IndexBuffer indexBuffer(vertexIndices.data(), sizeOfIndices);
	// IndexBuffer indexBuffer(vertexPosIndices, sizeof(vertexPosIndices));

	ShaderProgram basicShader{};
	basicShader.CompileShader("shader.vert", ShaderType::VERTEX);
	basicShader.CompileShader("shader.frag", ShaderType::FRAGMENT);
	basicShader.Link();
	basicShader.Validate();

	basicShader.Use();

	camera.GetTransform().SetPosition({0.f, 0.f, -10.f});
	basicShader.SetUniform<glm::mat4>("projectionMatrix", camera.GetProjectionMatrix());


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
	
	float cameraPosition[3]={0,0,-10.f};
	float cameraRotation[3]={0,0,0};

	while (!glfwWindowShouldClose(glfwWindow))
	{
		renderer.Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("ImGui Hello World!");
		ImGui::SliderFloat3("Camera Position", cameraPosition, -20.f, 20.0f);
		ImGui::SliderFloat3("Camera Rotation", cameraRotation, -180.f, 180.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		camera.GetTransform().SetRotation({cameraRotation[0], cameraRotation[1], cameraRotation[2]});
		camera.GetTransform().SetPosition({cameraPosition[0], cameraPosition[1], cameraPosition[2]});
		

		basicShader.SetUniform<glm::mat4>("viewMatrix", camera.GetUpdatedViewMatrix());
		basicShader.SetUniform<glm::mat4>("modelMatrix", glm::mat4{1.f});

		//Draw the bound buffers data
		renderer.Draw(vertexArrayObject, indexBuffer, basicShader);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
