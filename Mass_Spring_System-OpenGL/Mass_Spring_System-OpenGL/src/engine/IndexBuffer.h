#pragma once
#include "glad/glad.h"

class IndexBuffer
{
private:
	/* Renderer ID is the ID related to the renderer, in our case OpenGL */
	GLuint m_RendererID;
	GLuint m_Size; 

public:
	IndexBuffer() = default;
	IndexBuffer(const GLuint* data, GLuint size);
	~IndexBuffer();

	void Set(const GLuint* data, GLuint size);

	void Bind() const;
	void Unbind() const;

	inline GLuint GetCount() const { return m_Size; }
};

