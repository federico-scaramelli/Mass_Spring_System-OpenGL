#pragma once
#include "../engine/GameObjectUI.h"

class WindUI : public GameObjectUI
{
public:
	WindUI(const std::string& name) : GameObjectUI (name)
	{
		strcpy_s(m_LabelFullForceRadius, name.c_str());
		strcat_s (m_LabelFullForceRadius, " full force radius");
		strcpy_s(m_LabelAttenuationRadius, name.c_str());
		strcat_s (m_LabelAttenuationRadius, " attenuation radius");
		strcpy_s(m_LabelForce, name.c_str());
		strcat_s (m_LabelForce, " force multiplier");
		strcpy_s(m_LabelOffsetMaxValue, name.c_str());
		strcat_s (m_LabelOffsetMaxValue, " tilt factor");
	}

	void Draw() override
	{
		m_TransformUI->Draw();
		m_MaterialUI->Draw();

		ImGui::Text ("Wind parameters");
		ImGui::SliderFloat (m_LabelFullForceRadius, &m_FullForceRadiusData, 0.0f, 250);
		ImGui::SliderFloat (m_LabelAttenuationRadius, &m_AttenuationRadiusData, 0, 500);
		ImGui::SliderFloat (m_LabelForce, &m_ForceData, 0, 10000);
		ImGui::SliderFloat (m_LabelOffsetMaxValue, &m_OffsetMaxValueData, 0, 30);
	}

	float m_FullForceRadiusData = 5;
	float m_AttenuationRadiusData = 20;
	float m_ForceData = 20;
	float m_OffsetMaxValueData = 5;

private:
	char m_LabelFullForceRadius[50];
	char m_LabelAttenuationRadius[50];
	char m_LabelOffsetMaxValue[50];
	char m_LabelForce[50];
};
