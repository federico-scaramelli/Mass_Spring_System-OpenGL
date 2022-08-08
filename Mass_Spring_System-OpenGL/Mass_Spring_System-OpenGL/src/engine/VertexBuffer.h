#pragma once
#include <array>
#include <vector>

#include "Renderer.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"

class VertexBuffer
{
private:
	GLuint m_rendererID;
	GLsizei vertexBufferSize;

public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	void BindToVao(GLuint bindingIdx, GLuint offset, GLuint stride) const; 
	void Unbind() const;
};

