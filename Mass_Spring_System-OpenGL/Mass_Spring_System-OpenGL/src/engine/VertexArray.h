#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	GLuint m_bufferID;
	/* Current binding index */
	uint16_t vboCount = 0;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb);

	void Bind() const;
	void Unbind() const;
};
