#pragma once
#include <iostream>
#include <string>

#include "glad/glad.h"
#include "engine/Renderer.h"

void debugCallback(GLenum source,
                   GLenum type,
                   GLuint id,
                   GLenum severity,
                   GLsizei length,
                   const GLchar* message,
                   const void* param)
{
	std::cout << message << " " /*<< source << " " << type << " " << id << " " << severity*/ << std::endl;
}

void EnableDebug()
{
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEBUG_OUTPUT);

	glDebugMessageCallback(debugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}
