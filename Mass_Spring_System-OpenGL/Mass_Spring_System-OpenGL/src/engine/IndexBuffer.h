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

	void SetData(const GLuint* data, GLuint size);

	void Bind() const;
	void Unbind() const;

	[[nodiscard]] inline GLuint GetCount() const { return m_Size; }
};

