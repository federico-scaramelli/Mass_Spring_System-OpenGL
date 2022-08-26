#include "Wind.h"
#include "../engine/Scene.h"
#include "WindUI.h"
#include "../engine/Camera.h"

Wind::Wind (GLfloat fullForceRadius, GLfloat attenuationRadius, GLfloat forceMult) :
	Primitive ("Wind", CONE, 20, 10, 30)
{
	delete m_GameObjectUI;
	m_GameObjectUI = new WindUI (name);

	windUI = dynamic_cast<WindUI*> (m_GameObjectUI);

	SetFullForceRadius (fullForceRadius);
	SetAttenuationRadius (attenuationRadius);
	SetForceMultiplier (forceMult);
}

void Wind::GenerateUI () { Primitive::GenerateUI(); }
void Wind::Create ()
{ 
    SetupGraphicsShader (Solid);
	GenerateUI();
}

void Wind::Update()
{
	//Update the alternative rotation
	glm::vec3 offset = {
		(Utils::unif(Utils::generator) * 2) - 1,
		(Utils::unif(Utils::generator) * 2) - 1,
		(Utils::unif(Utils::generator) * 2) - 1
	};
	offset *= m_OffsetMaxValue;

	alternativeRotation = GetTransform().GetRotation() + offset;

	glm::mat4 viewMatrix = Scene::GetInstance()->GetCamera()->GetUpdatedViewMatrix();
	auto modelViewMatrix = viewMatrix * GetTransform().GetUpdatedCustomRotationModelMatrix(alternativeRotation);
	GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);
}

void Wind::UpdateWithUI ()
{
	m_Transform.UpdateWithUI();
	m_Mesh.GetMaterial().UpdateWithUI();
	
	m_FullForceRadius = windUI->m_FullForceRadiusData;
	m_AttenuationRadius = windUI->m_AttenuationRadiusData;
	m_ForceMultiplier = windUI->m_ForceData;

	m_OffsetMaxValue = windUI->m_OffsetMaxValueData;
	m_ReferenceDistance = windUI->m_refDistanceData;
}

void Wind::SetFullForceRadius (GLfloat fullForceRadius)
{
	m_FullForceRadius = fullForceRadius;
	windUI->m_FullForceRadiusData = fullForceRadius;
}

void Wind::SetAttenuationRadius (GLfloat attenuationRadius)
{
	m_AttenuationRadius = attenuationRadius;
	windUI->m_AttenuationRadiusData = attenuationRadius;
}

void Wind::SetForceMultiplier (GLfloat forceMultiplier)
{
	m_ForceMultiplier = forceMultiplier;
	windUI->m_ForceData = forceMultiplier;
}
