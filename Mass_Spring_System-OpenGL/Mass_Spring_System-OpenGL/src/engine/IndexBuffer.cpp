#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count)
	: m_Count(count)
{
	GLCall(glGenBuffers(2, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW)); 
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID))
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}