#pragma once
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Utils.h"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Mass Spring System"

GLFWwindow* CreateWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR_VERSION);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) 
	{
		std::cerr << "Unable to load OpenGL functions!" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}
