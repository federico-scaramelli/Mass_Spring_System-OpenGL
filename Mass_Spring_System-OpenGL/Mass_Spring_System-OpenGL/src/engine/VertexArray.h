#pragma once
#include "glad/glad.h"

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
private:
	GLuint m_bufferID;
	// uint16_t vboCount = 0;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vertexBuffer, VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
