#include "DebugLogger.h"

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


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}