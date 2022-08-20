#pragma once
#include <vector>

#include "RendererUI.h"

class VertexArray;
class IndexBuffer;
class ShaderProgram;

class Renderer {
public:

	void Draw(const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const;
	void Clear() const;

	void DrawUI();

	bool wireframe = true;
	bool backface = true;

	RendererUI UI;
};
