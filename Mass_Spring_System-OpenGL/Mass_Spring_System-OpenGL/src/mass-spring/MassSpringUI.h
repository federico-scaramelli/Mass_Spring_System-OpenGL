#pragma once
#include "../engine/GameObjectUI.h"

class MassSpringUI : public GameObjectUI
{
public:
	inline static const char* massSpringsList[10];
	inline static int selectedMassSpring = 0;

	MassSpringUI(const char* name) : GameObjectUI (name)
	{
		strcpy_s(m_LabelStiffness, name);
		strcat_s (m_LabelStiffness, " elastic stiffness");

		strcpy_s(m_LabelDamping, name);
		strcat_s (m_LabelDamping, " euler damping");

		strcpy_s(m_LabelParticleMass, name);
		strcat_s (m_LabelParticleMass, " particle mass");

		strcpy_s(m_LabelGravity, name);
		strcat_s (m_LabelGravity, " gravity");

		strcpy_s(m_LabelConstSheering, name);
		strcat_s (m_LabelConstSheering, " const sheering");

		strcpy_s(m_LabelConstBending, name);
		strcat_s (m_LabelConstBending, " const bending");

		strcpy_s(m_LabelCorrectionDumping, name);
		strcat_s (m_LabelCorrectionDumping, " distance dumping");

		strcpy_s(m_LabelConstraintDistanceMult, name);
		strcat_s (m_LabelConstraintDistanceMult, " distance length mult");

		strcpy_s(m_LabelSelfCollisionDistanceMult, name);
		strcat_s (m_LabelSelfCollisionDistanceMult, " self collision mult");

		strcpy_s(m_LabelSphereRepulsionDistMult, name);
		strcat_s (m_LabelSphereRepulsionDistMult, " sphere repulsion mult");

		strcpy_s(m_LabelSphereRepulsionDamping, name);
		strcat_s (m_LabelSphereRepulsionDamping, " sphere dumping");

	}

	void Draw() override
	{
		GameObjectUI::Draw();

		ImGui::Dummy ({ 0, 10 });
		ImGui::Text ("Spring parameters");
		ImGui::SliderFloat (m_LabelStiffness, &m_StiffnessData, 0.0f, 100000);
		ImGui::SliderFloat (m_LabelDamping, &m_DampingData, 0, 6);
		ImGui::SliderFloat (m_LabelParticleMass, &m_ParticleMassData, 1.0f, 1000);
		ImGui::SliderFloat (m_LabelGravity, &m_GravityData, -10000.f, -9.81f);
		ImGui::SliderFloat (m_LabelConstSheering, &m_ConstSheeringData, 0.2f, 2);
		ImGui::SliderFloat (m_LabelConstBending, &m_ConstBendingData, 0.2f, 2);

		ImGui::SliderFloat (m_LabelCorrectionDumping, &m_CorrectionDumpingData, 0, 0.3f);
		ImGui::SliderFloat (m_LabelConstraintDistanceMult, &m_ConstraintDistanceMultData, 1, 1.3f);
		ImGui::SliderFloat (m_LabelSelfCollisionDistanceMult, &m_SelfCollisionDistanceMultData, 0.8f, 1.0f);
		ImGui::SliderFloat (m_LabelSphereRepulsionDistMult, &m_SphereRepulsionDistMultData, 0.2f, 1.3f);
		ImGui::SliderFloat (m_LabelSphereRepulsionDamping, &m_SphereRepulsionDampingData, 0, 1);
	}

	float m_StiffnessData = 10;
	float m_DampingData = 0.98f;
	float m_ParticleMassData = 1;
	float m_GravityData = -1500;
	float m_ConstSheeringData = 1;
	float m_ConstBendingData = 1;

	float m_CorrectionDumpingData = 0.25f;
	float m_ConstraintDistanceMultData = 1.1f;
	float m_SelfCollisionDistanceMultData = 1.0f;
	float m_SphereRepulsionDistMultData = 0.33f;
	float m_SphereRepulsionDampingData = 0.95f;

private:
	char m_LabelStiffness[50];
	char m_LabelDamping[50];
	char m_LabelParticleMass[50];
	char m_LabelGravity[50];
	char m_LabelConstSheering[50];
	char m_LabelConstBending[50];

	char m_LabelCorrectionDumping[50];
	char m_LabelConstraintDistanceMult[50];
	char m_LabelSelfCollisionDistanceMult[50];
	char m_LabelSphereRepulsionDistMult[50];
	char m_LabelSphereRepulsionDamping[50];
};