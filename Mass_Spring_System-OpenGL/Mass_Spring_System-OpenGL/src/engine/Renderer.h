#pragma once
#include "RendererUI.h"

class VertexArray;
class IndexBuffer;
class ShaderProgram;

class Renderer
{
private:
	Renderer () : UI() {}
	~Renderer () { delete instance; }
	static Renderer* instance;

public:
	Renderer (Renderer& other) = delete;
	Renderer operator= (const Renderer&) = delete;

	static Renderer* GetInstance ();

	void Draw (const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const;
	void Clear () const;

	void DrawUI ();

	bool wireframe = false;
	bool backface = true;

	RendererUI UI;
};
