#pragma once
#include "MaterialUI.h"

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
