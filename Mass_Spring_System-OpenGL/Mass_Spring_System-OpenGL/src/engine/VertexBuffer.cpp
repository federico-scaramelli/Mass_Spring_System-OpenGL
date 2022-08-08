#include "VertexBuffer.h"
#include "Renderer.h"
#include "../Utils.h"
#include "glm/vec3.hpp"

VertexBuffer::VertexBuffer(const void* data, const GLuint size)
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID))
}

VertexBufferFormat& VertexBuffer::GetFormat()
{
	return m_format;
}

#pragma region FormatManagement
template <typename T>
void VertexBuffer::SetFormat(GLuint count)
{
	static_assert(Utils::falseTemplate<T>);
}

template <>
void VertexBuffer::SetFormat<GLfloat>(GLuint count)
{
	m_format = VertexBufferFormat{GL_FLOAT, count, GL_FALSE, 0};
	m_format.stride = sizeof(GLfloat) * count;
}

template <>
void VertexBuffer::SetFormat<glm::vec3>(GLuint count)
{
	m_format = VertexBufferFormat{GL_FLOAT_VEC3, count, GL_FALSE, 0};
	m_format.stride = sizeof(glm::vec3) * count;
}

template <>
void VertexBuffer::SetFormat<GLuint>(GLuint count)
{
	m_format = VertexBufferFormat{GL_UNSIGNED_INT, count, GL_FALSE, 0};
	m_format.stride = sizeof(GLuint) * count;
}

template <>
void VertexBuffer::SetFormat<GLubyte>(GLuint count)
{
	m_format = VertexBufferFormat{GL_UNSIGNED_BYTE, count, GL_FALSE, 0};
	m_format.stride = sizeof(GLubyte) * count;
}

void VertexBuffer::SetFormat(VertexBufferFormat& format)
{
	m_format = format;
}
#pragma endregion

void VertexBuffer::Bind(GLuint bindingIdx, GLuint offset) const
{
	GLCall(glBindVertexBuffer(bindingIdx, m_rendererID, offset, m_format.stride));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindVertexBuffer(0, m_rendererID, 0, 0));
}
