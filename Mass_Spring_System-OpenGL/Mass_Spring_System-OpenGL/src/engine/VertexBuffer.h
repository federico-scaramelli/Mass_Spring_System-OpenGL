#pragma once
#include <array>
#include <vector>

#include "glad/glad.h"
#include "glm/vec3.hpp"

class VertexBuffer
{
private:
	GLuint m_vboID;
	GLsizei m_vboSize;

public:
	VertexBuffer() = default;
	VertexBuffer(const void* data, GLsizei size);

	~VertexBuffer();

	void SetData(const void* data, GLsizei size);
	void UpdateData(const void* data, GLsizei size);

	void BindToVao(GLuint vaoID, GLuint bindingPoint, GLsizei stride) const; 
	void Unbind(GLuint vaoID, GLuint bindingPoint) const;

	GLuint GetID() { return m_vboID; }
	GLsizei GetSize() { return m_vboSize; }
};

