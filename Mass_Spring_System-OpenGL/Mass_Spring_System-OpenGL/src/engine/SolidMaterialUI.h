#pragma once
#include "MaterialUI.h"

class SolidMaterialUI : public MaterialUI
{
public:
    SolidMaterialUI(const char* name)
	{
		strcpy_s(m_LabelColor, name);
		strcat_s (m_LabelColor, " material color");
	}
	
	void Draw() override
	{
		ImGui::Text ("Material - Solid Color shader");
		ImGui::ColorEdit3 (m_LabelColor, m_ColorData);
	}

	void SetValues(glm::vec3 colorData)
	{
		m_ColorData[0] = colorData[0];
		m_ColorData[1] = colorData[1];
		m_ColorData[2] = colorData[2];
	}

	float m_ColorData[3] = {0.5f, 0.5f, 0.5f};

private:
	char m_LabelColor[50];
};