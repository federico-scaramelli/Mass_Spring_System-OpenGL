#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <iostream>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const
{
	shader.Use();
	vao.Bind();
	indexBuffer.Bind();

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (backface)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	glEnable(GL_DEPTH_TEST); 
	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
	//Clear the color buffer
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawUI()
{
	UI.DrawUI();
}
