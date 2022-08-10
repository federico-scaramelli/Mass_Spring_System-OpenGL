#pragma once
#include <vector>

class VertexArray;
class IndexBuffer;
class ShaderProgram;

struct UISliderInt {
	const char* label;
	int* data;
	int min;
	int max;
};

struct UISlider {
	const char* label;
	float* data;
	float min;
	float max;
};

struct UICheckbox {
	const char* label;
	bool* data;
};

class Renderer {
public:
	void Draw(const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const;

	void Clear() const;

	void AddFloatSliderUI(const char* label, float* data, float min, float max);
	void DrawBoolCheckboxUI(const char* label, bool* data);
	void AddBoolCheckboxUI(const char* label, bool* data);
	void AddIntSliderUI(const char* label, int* data, int min, int max);
	void DrawIntSliderUI(const char* label, int* data, int min, int max);
	void DrawUI();

	bool wireframe;
	bool backface;

private:
	std::vector<UISliderInt> UISlidersInt;
	std::vector<UISlider> UISliders;
	std::vector<UICheckbox> UICheckboxes;
	void DrawFloatSliderUI(const char* label, float* data, float min, float max);
};
