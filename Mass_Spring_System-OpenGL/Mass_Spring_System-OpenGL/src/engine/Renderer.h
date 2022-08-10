#pragma once
#include <vector>

class VertexArray;
class IndexBuffer;
class ShaderProgram;

struct UISlider
{
	const char* label;
	float* data;
	float min;
	float max;
};

class Renderer
{
public:
	void Draw (const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const;

	void Clear () const;

	void AddFloatSliderUI (const char* label, float* data, float min, float max);
	void DrawUI ();

private:
	std::vector<UISlider> UISliders;
	void DrawFloatSliderUI (const char* label, float* data, float min, float max);
};