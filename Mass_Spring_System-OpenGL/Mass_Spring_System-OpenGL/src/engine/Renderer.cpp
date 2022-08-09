#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <iostream>

#include "glm/fwd.hpp"

void Renderer::Draw (const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const
{
	shader.Use();
	vao.Bind();
	indexBuffer.Bind();
	glDrawElements (GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear () const
{
	//Clear the color buffer
	glClearColor (0.07f, 0.13f, 0.17f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT);
}
