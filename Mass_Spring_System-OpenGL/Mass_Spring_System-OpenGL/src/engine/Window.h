#pragma once
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Utils.h"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Mass Spring System"

class Window
{
public:
	Window();
	~Window();

	GLFWwindow* GetGLFWWindow() { return window; }

	GLfloat GetAspectRatio() const
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		return static_cast<GLfloat>(width) / height;
	}

private:
	GLFWwindow* window = nullptr;
	void InitializeImGui();
};