#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <iostream>

#include "glm/fwd.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void Renderer::Draw (const VertexArray& vao, const IndexBuffer& indexBuffer, const ShaderProgram& shader) const
{
	shader.Use();
	vao.Bind();
	indexBuffer.Bind();

	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (backface) {
		glDisable(GL_CULL_FACE); 
	} else {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);  
	}
	
	glDrawElements (GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear () const
{
	//Clear the color buffer
	glClearColor (0.07f, 0.13f, 0.17f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT);
}

void Renderer::AddFloatSliderUI (const char* label, float* data, float min, float max)
{
	UISliders.push_back ({label, data, min, max});
}

void Renderer::DrawFloatSliderUI (const char* label, float* data, float min, float max)
{
	ImGui::SliderFloat3 (label, data, min, max);
}

void Renderer::DrawBoolCheckboxUI (const char* label, bool* data)
{
	ImGui::Checkbox (label, data);
}

void Renderer::AddBoolCheckboxUI (const char* label, bool* data)
{
	UICheckboxes.push_back ({label, data});
}

void Renderer::DrawUI ()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin ("ImGui Hello World!");

	for (UISlider slider : UISliders) {
		DrawFloatSliderUI (slider.label, slider.data, slider.min, slider.max);
	}

	for (UICheckbox checkbox : UICheckboxes) {
		DrawBoolCheckboxUI (checkbox.label, checkbox.data);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}