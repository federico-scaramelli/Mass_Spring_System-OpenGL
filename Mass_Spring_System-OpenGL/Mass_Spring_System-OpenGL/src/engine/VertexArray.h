#pragma once
#include "glad/glad.h"

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
private:
	GLuint m_vaoID;
public:
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};
