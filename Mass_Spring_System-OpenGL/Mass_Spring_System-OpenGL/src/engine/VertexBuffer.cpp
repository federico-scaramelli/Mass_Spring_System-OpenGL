#include "VertexBuffer.h"

#include "Renderer.h"
#include "../Utils.h"
#include "glm/vec3.hpp"

VertexBuffer::VertexBuffer(const void* data, const size_t size)
{
	vertexBufferSize=size;

	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::BindToVao(GLuint bindingIdx, GLuint offset, GLuint stride) const
{
	// glBindVertexBuffer(bindingIdx, m_rendererID, offset, stride);
	glVertexArrayVertexBuffer(bindingIdx, /*slot*/ 0, m_rendererID, /*offset*/ 0, /*stride*/ stride);
}

void VertexBuffer::Unbind() const
{
	glBindVertexBuffer(0, m_rendererID, 0, 0);
}
