#pragma once

class VertexArray;
class IndexBuffer;
class ShaderProgram;

class Renderer
{
public:
	void Draw (const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const;

	void Clear () const;
};
