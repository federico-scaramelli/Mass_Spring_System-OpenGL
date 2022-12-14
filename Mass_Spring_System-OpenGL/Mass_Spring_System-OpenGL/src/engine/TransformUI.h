#pragma once
#include "glm/vec2.hpp"
#include "imgui/imgui.h"
#include <string>

class TransformUI 
{
public:
	TransformUI();
	TransformUI (const std::string& name)
	{
		strcpy_s (m_LabelPosition, name.c_str());
		strcat_s (m_LabelPosition, " position");

		strcpy_s (m_LabelRotation, name.c_str());
		strcat_s (m_LabelRotation, " rotation");
	}

	void SetPositionRange(const glm::vec2 minMax) { m_PositionMinMax = minMax; }
	void SetRotationRange(const glm::vec2 minMax) { m_RotationMinMax = minMax; }

	void Draw()
	{
		ImGui::Text ("Transform");
		ImGui::SliderFloat3 (m_LabelPosition,
		                     m_PositionData,
		                     m_PositionMinMax.x,
		                     m_PositionMinMax.y);
		ImGui::SliderFloat3 (m_LabelRotation,
		                     m_RotationData,
		                     m_RotationMinMax.x,
		                     m_RotationMinMax.y);
	}

	float m_PositionData[3] = {0, 0, 0};
	float m_RotationData[3] = {0, 0, 0};

private:
	char m_LabelPosition[40];
	char m_LabelRotation[40];

	glm::vec2 m_PositionMinMax {-500, 500};
	glm::vec2 m_RotationMinMax {-180, 180};
};
