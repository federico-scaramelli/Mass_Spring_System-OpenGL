#pragma once
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../Utils.h"

#define OPENGL_MAJOR_VERSION 4
#define OPENGL_MINOR_VERSION 6
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Mass Spring System"

class Window
{
public:
	Window();
	~Window();

	GLFWwindow* GetGLFWWindow() { return window; }

	GLfloat GetAspectRatio() const { return WINDOW_WIDTH / WINDOW_WIDTH; }

private:
	GLFWwindow* window = nullptr;
};