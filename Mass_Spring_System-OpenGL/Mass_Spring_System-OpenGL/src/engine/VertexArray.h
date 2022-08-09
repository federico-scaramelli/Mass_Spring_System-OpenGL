#pragma once
#include "glad/glad.h"

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
private:
	GLuint m_vaoID;
	// uint16_t vboCount = 0;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
