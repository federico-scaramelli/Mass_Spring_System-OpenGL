#pragma once
#include "glm/vec2.hpp"
#include "imgui/imgui.h"

class TransformUI 
{
public:
	TransformUI();
	TransformUI (const char* name)
	{
		strcpy_s (m_LabelPosition, name);
		strcat_s (m_LabelPosition, " position");

		strcpy_s (m_LabelRotation, name);
		strcat_s (m_LabelRotation, " rotation");
	}

	void SetPositionRange(glm::vec2 minMax) { m_PositionMinMax = minMax; }
	void SetRotationRange(glm::vec2 minMax) { m_RotationMinMax = minMax; }

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

	glm::vec2 m_PositionMinMax {-250, 250};
	glm::vec2 m_RotationMinMax {-250, 250};
};
