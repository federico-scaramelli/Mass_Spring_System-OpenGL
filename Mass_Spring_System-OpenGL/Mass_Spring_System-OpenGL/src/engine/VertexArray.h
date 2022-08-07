#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	GLuint m_bufferID;
	uint16_t i=0;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb);

	void Bind() const;
	void Unbind() const;
};