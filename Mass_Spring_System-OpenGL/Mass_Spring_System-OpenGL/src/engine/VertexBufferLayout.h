#pragma once
#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "DebugLogger.h"
#include "Utils.h"

struct VertexBufferElement
{
	GLuint type;
	GLuint count; 
	GLboolean normalized;

	static GLuint GetSizeOfType(GLuint type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			case GL_FLOAT_VEC3: return 12;
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	GLsizei m_Stride;

public:
	VertexBufferLayout() : m_Stride(0)
	{
		Push<GLfloat> (4); //0
		Push<GLfloat> (4); //1
		Push<GLfloat> (4); //2
		Push<GLfloat> (4); //3
		Push<GLfloat> (4); //4
		Push<GLfloat> (4); //5
	}

	const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	GLsizei GetStride() const { return m_Stride; }

#pragma region Push templates

	template<typename T>
	void Push(GLuint count)
	{
		static_assert(Utils::falseTemplate<T>);
	}

	template<>
	void Push<GLfloat>(GLuint count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec3>(GLuint count)
	{
		m_Elements.push_back({ GL_FLOAT_VEC3, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT_VEC3);
	}

	template<>
	void Push<GLuint>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<GLubyte>(GLuint count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

#pragma endregion
};

