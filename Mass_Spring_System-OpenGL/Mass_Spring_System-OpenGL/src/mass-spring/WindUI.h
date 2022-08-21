#pragma once
#include "../engine/GameObjectUI.h"

class WindUI : public GameObjectUI
{
public:
	WindUI(const char* name) : GameObjectUI (name)
	{
		strcpy_s(m_LabelFullForceRadius, name);
		strcat_s (m_LabelFullForceRadius, " full force radius");
		strcpy_s(m_LabelAttenuationRadius, name);
		strcat_s (m_LabelAttenuationRadius, " attenuation radius");
		strcpy_s(m_LabelForce, name);
		strcat_s (m_LabelForce, " force multiplier");
	}

	void Draw() override
	{
		m_TransformUI->Draw();

		ImGui::Text ("Wind parameters");
		ImGui::SliderFloat (m_LabelFullForceRadius, &m_FullForceRadiusData, 0.1f, 250);
		ImGui::SliderFloat (m_LabelAttenuationRadius, &m_AttenuationRadiusData, 0, 500);
		ImGui::SliderFloat (m_LabelForce, &m_ForceData, 0, 10000);
	}

	float m_FullForceRadiusData = 5;
	float m_AttenuationRadiusData = 20;
	float m_ForceData = 20;

private:
	char m_LabelFullForceRadius[50];
	char m_LabelAttenuationRadius[50];
	char m_LabelForce[50];
};
