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
	VertexBuffer(const void* data, GLsizei size);
	~VertexBuffer();

	void BindToVao(GLuint vaoID, GLuint bindingPoint, GLsizei stride) const; 
	void Unbind(GLuint vaoID, GLuint bindingPoint) const;
};

