#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::Window()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (window == NULL) 
	{
		throw std::runtime_error("Failed to create GLFW window!");
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) 
	{
		throw std::runtime_error("Unable to load OpenGL functions!");
	}

	glfwSetWindowAspectRatio(window, 16, 9);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);

	glfwSwapInterval( 0 );

	InitializeImGui ();
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Window::InitializeImGui ()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL (GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init ("#version 460");
}