#pragma once
#include <iostream>
#include <string>
#include "glad/glad.h"

// Support class for debugging and logging

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();					\
		  x;										\
		  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void debugCallback(GLenum source,
                   GLenum type,
                   GLuint id,
                   GLenum severity,
                   GLsizei length,
                   const GLchar* message,
                   const void* param);

void EnableDebug();
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);