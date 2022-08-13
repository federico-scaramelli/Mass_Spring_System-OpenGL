#pragma once

#include "ShaderProgramCompute.h"

#include <string>
#include <vector>

#include "Renderer.h"
#include "glm/vec3.hpp"

class ShaderProgram;

class Material
{
private:
	ShaderProgram m_Shader{};
	ShaderProgramCompute m_ComputeShader{};

	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;
	glm::vec3 m_AmbientColor;
	float m_Shininess;

	float UI_Diffuse[3] = {0.5f, 0.5f, 0.5f};
	float UI_Specular[3] = {0.5f, 0.5f, 0.5f};
	float UI_Ambient[3] = {0.5f, 0.5f, 0.5f};
	float UI_Shininess = 1;

	char UILabel_Diffuse[50];
	char UILabe_Specular[50];
	char UILabe_Ambient[50];
	char UILabe_Shininess[50];

public:
	Material(glm::vec3 diffuseColor = {0.5f, 0.5f, 0.5f}, 
			 glm::vec3 specularColor = {0.5f, 0.5f, 0.5f}, 
			 glm::vec3 ambientColor = {0.5f, 0.5f, 0.5f}, 
			 float shininess = 1) :
	m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), m_AmbientColor(ambientColor), m_Shininess(shininess) { }

	void SetProperties(glm::vec3 diffuseColor, 
					   glm::vec3 specularColor, 
					   glm::vec3 ambientColor, 
					   float shininess)
	{
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AmbientColor = ambientColor;
		m_Shininess = shininess;
	}

	void CreateShaderProgram(std::vector<std::pair<std::string, ShaderType>> pairList);

	void CreateComputeShaderProgram(std::vector<std::pair<std::string, ShaderType>> pairList);

	ShaderProgram& GetShader() { return m_Shader; }

	ShaderProgramCompute& GetComputeShader() { return m_ComputeShader; }

	glm::vec3 GetAmbient() { return m_AmbientColor; }
	glm::vec3 GetDiffuse() { return m_DiffuseColor; }
	glm::vec3 GetSpecular() { return m_SpecularColor; }
	float GetShininess() { return m_Shininess; }

	void GenerateUI(Renderer& renderer, const char* name)
	{
		strcpy_s(UILabel_Diffuse, name);
		strcat_s (UILabel_Diffuse, " diffuse");
		renderer.AddFloat3SliderUI (UILabel_Diffuse, UI_Diffuse, 0.f, 1.f);
		
		strcpy_s (UILabe_Specular, name);
		strcat_s (UILabe_Specular, " specular");
		renderer.AddFloat3SliderUI (UILabe_Specular, UI_Specular, 0.f, 1.f);
		
		strcpy_s (UILabe_Ambient, name);
		strcat_s (UILabe_Ambient, " ambient");
		renderer.AddFloat3SliderUI (UILabe_Ambient, UI_Ambient, 0.f, 1.f);
		
		strcpy_s (UILabe_Shininess, name);
		strcat_s (UILabe_Shininess, " shininess");
		renderer.AddFloatSliderUI (UILabe_Shininess, &UI_Shininess, 0, 100);
	}

	void UpdateWithUI ()
	{
		m_DiffuseColor = {UI_Diffuse[0], UI_Diffuse[1], UI_Diffuse[2]};
		m_SpecularColor = {UI_Specular[0], UI_Specular[1], UI_Specular[2]};
		m_AmbientColor = {UI_Ambient[0], UI_Ambient[1], UI_Ambient[2]};
		m_Shininess = UI_Shininess;
	}
};

