#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, const GLuint size, const VertexBufferFormat& format)
{
	m_format=format;

	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); 
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID))
}

VertexBufferFormat& VertexBuffer::GetFormat() {
	return m_format;
}

void VertexBuffer::Bind(GLuint bindingIdx, GLuint offset, GLuint stride) const
{
	GLCall(glBindVertexBuffer(bindingIdx, m_rendererID, offset, stride));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindVertexBuffer(0, m_rendererID, 0, 0));
}