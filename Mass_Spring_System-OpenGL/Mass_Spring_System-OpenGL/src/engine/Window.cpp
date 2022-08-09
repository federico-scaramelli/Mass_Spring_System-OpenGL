#include "Window.h"

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

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}