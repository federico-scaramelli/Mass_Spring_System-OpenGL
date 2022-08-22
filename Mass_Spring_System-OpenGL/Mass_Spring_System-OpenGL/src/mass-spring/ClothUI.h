#pragma once
#include "../engine/GameObjectUI.h"

class ClothUI : public GameObjectUI
{
public:
	ClothUI(const char* name) : GameObjectUI (name)
	{
		strcpy_s(m_LabelStiffness, name);
		strcat_s (m_LabelStiffness, " elastic stiffness");

		strcpy_s(m_LabelDamping, name);
		strcat_s (m_LabelDamping, " euler damping");

		strcpy_s(m_LabelParticleMass, name);
		strcat_s (m_LabelParticleMass, " particle mass");

		strcpy_s(m_LabelConstSheering, name);
		strcat_s (m_LabelConstSheering, " const sheering");

		strcpy_s(m_LabelConstBending, name);
		strcat_s (m_LabelConstBending, " const bending");

	}

	void Draw() override
	{
		m_TransformUI->Draw();
		m_MaterialUI->Draw();

		ImGui::Text ("Spring parameters");
		ImGui::SliderFloat (m_LabelStiffness, &m_StiffnessData, 0.0f, 10000);
		ImGui::SliderFloat (m_LabelDamping, &m_DampingData, 0, 1);
		ImGui::SliderFloat (m_LabelParticleMass, &m_ParticleMassData, 0, 1000);
		ImGui::SliderFloat (m_LabelConstSheering, &m_ConstSheeringData, 0, 2);
		ImGui::SliderFloat (m_LabelConstBending, &m_ConstBendingData, 0, 2);
	}

	float m_StiffnessData = 10;
	float m_DampingData = 0.98f;
	float m_ParticleMassData = 1;
	float m_ConstSheeringData = 1;
	float m_ConstBendingData = 1;

private:
	char m_LabelStiffness[50];
	char m_LabelDamping[50];
	char m_LabelParticleMass[50];
	char m_LabelConstSheering[50];
	char m_LabelConstBending[50];
};
