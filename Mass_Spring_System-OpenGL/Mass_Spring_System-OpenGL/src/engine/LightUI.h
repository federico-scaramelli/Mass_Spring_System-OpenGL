#pragma once
#include "GameObjectUI.h"

class LightUI : public GameObjectUI
{
public:
	LightUI(const char* name) : GameObjectUI (name)
	{
		strcpy_s(m_LabelDiffuse, name);
		strcat_s (m_LabelDiffuse, " diffuse");

		strcpy_s (m_LabelAmbient, name);
		strcat_s (m_LabelAmbient, " ambient");
	}

	void Draw() override
	{
		m_TransformUI->Draw();

		ImGui::Text ("Light");
		ImGui::ColorEdit3 (m_LabelDiffuse, m_LightDiffuseData);
		ImGui::ColorEdit3 (m_LabelAmbient, m_LightAmbientData);
	}

	float m_LightDiffuseData[3] = {1, 1, .5f};
	float m_LightAmbientData[3] = {0, 0, 0};

private:
	char m_LabelDiffuse[50];
	char m_LabelAmbient[50];
};
