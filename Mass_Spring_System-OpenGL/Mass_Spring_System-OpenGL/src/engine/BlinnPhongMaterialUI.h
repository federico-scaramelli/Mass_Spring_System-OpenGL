#pragma once
#include "MaterialUI.h"
#include "glm/vec3.hpp"

class BlinnPhongMaterialUI : public MaterialUI
{
public:
	BlinnPhongMaterialUI(const char* name)
	{
		strcpy_s(m_LabelDiffuse, name);
		strcat_s (m_LabelDiffuse, " material diffuse");
		
		strcpy_s (m_LabelSpecular, name);
		strcat_s (m_LabelSpecular, " material specular");
		
		strcpy_s (m_LabelAmbient, name);
		strcat_s (m_LabelAmbient, " material ambient");
		
		strcpy_s (m_LabelShininess, name);
		strcat_s (m_LabelShininess, " material shininess");
	}

	void SetShininessRange(glm::vec2 minMax) { m_ShininessMinMax = minMax; }
	
	void Draw() override
	{
		ImGui::Text ("Material - Blinn Phong shader");
		ImGui::ColorEdit3 (m_LabelDiffuse, m_DiffuseData);
		ImGui::ColorEdit3 (m_LabelSpecular, m_SpecularData);
		ImGui::ColorEdit3 (m_LabelAmbient, m_AmbientData);
		ImGui::SliderFloat (m_LabelShininess, &m_ShininessData, m_ShininessMinMax.x, m_ShininessMinMax.y);
	}

	void SetValues(glm::vec3 diffuseData, glm::vec3 specularData, glm::vec3 ambientData, float shininessData)
	{
		m_DiffuseData[0] = diffuseData[0];
		m_DiffuseData[1] = diffuseData[1];
		m_DiffuseData[2] = diffuseData[2];
		m_SpecularData[0] = specularData[0];
		m_SpecularData[1] = specularData[1];
		m_SpecularData[2] = specularData[2];
		m_AmbientData[0] = ambientData[0];
		m_AmbientData[1] = ambientData[1];
		m_AmbientData[2] = ambientData[2];
		m_ShininessData = shininessData;
	}

	float m_DiffuseData[3] = {0.5f, 0.5f, 0.5f};
	float m_SpecularData[3] = {0.5f, 0.5f, 0.5f};
	float m_AmbientData[3] = {0.5f, 0.5f, 0.5f};
	float m_ShininessData = 30;

private:
	char m_LabelDiffuse[50];
	char m_LabelSpecular[50];
	char m_LabelAmbient[50];
	char m_LabelShininess[50];

	glm::vec2 m_ShininessMinMax {0, 50};
};
