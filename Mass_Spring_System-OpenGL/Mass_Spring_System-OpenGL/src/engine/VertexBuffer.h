#pragma once
#include "glad/glad.h"

class VertexBuffer
{
private:
	/* Renderer ID is the ID related to the renderer, in our case OpenGL */
	GLuint m_rendererID;

public:
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind(GLuint bindingIdx, GLuint offset, GLuint stride) const; 
	void Unbind() const;
};

