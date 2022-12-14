#pragma once
#include "../engine/GameObjectUI.h"

// Custom UI for wind object presenting the parameters to control wind
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
		strcpy_s(m_LabelRefDistance, name.c_str());
		strcat_s (m_LabelRefDistance, " reference distance");
	}

	void Draw() override
	{
		GameObjectUI::Draw();

		ImGui::Dummy ({ 0, 10 });
		ImGui::Text ("Wind parameters");
		ImGui::SliderFloat (m_LabelFullForceRadius, &m_FullForceRadiusData, 0.0f, 250);
		ImGui::SliderFloat (m_LabelAttenuationRadius, &m_AttenuationRadiusData, 0, 500);
		ImGui::SliderFloat (m_LabelForce, &m_ForceData, 10, 10000);
		ImGui::SliderFloat (m_LabelOffsetMaxValue, &m_OffsetMaxValueData, 0, 30);
		ImGui::SliderFloat (m_LabelRefDistance, &m_refDistanceData, 100, 1100);
	}

	float m_FullForceRadiusData = 5;
	float m_AttenuationRadiusData = 20;
	float m_ForceData = 200;
	float m_OffsetMaxValueData = 5;
	float m_refDistanceData = 1000;

private:
	char m_LabelFullForceRadius[50];
	char m_LabelAttenuationRadius[50];
	char m_LabelOffsetMaxValue[50];
	char m_LabelForce[50];
	char m_LabelRefDistance[50];
};
