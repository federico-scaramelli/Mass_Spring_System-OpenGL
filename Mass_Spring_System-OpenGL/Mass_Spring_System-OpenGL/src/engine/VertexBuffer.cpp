#include "VertexBuffer.h"

#include "Utils.h"
#include "glm/vec3.hpp"

VertexBuffer::VertexBuffer(const void* data, const GLsizei size)
{
	m_vboSize = size;

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_vboID);
}

void VertexBuffer::SetData (const void* data, GLsizei size)
{
	m_vboSize = size;

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::UpdateData (const void* data, GLsizei size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::BindToVao(GLuint vaoID, GLuint bindingPoint, GLsizei stride) const
{
	glVertexArrayVertexBuffer(vaoID, bindingPoint, m_vboID, /* offset */ 0, stride);
}

void VertexBuffer::Unbind(GLuint vaoID, GLuint bindingPoint) const
{
	throw std::runtime_error("Not implemented function");

	//glVertexArrayVertexBuffer(vaoID, bindingPoint, m_vboID, 0, 0);
	
	// TODO: I'm not sure it's unbinding..
	// glBindVertexBuffer(0, m_vboID, 0, 0);
}
