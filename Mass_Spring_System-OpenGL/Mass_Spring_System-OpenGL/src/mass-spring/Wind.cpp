#include "Wind.h"
#include "WindUI.h"

Wind::Wind (GLfloat fullForceRadius, GLfloat attenuationRadius, GLfloat forceMult) :
	Primitive ("Wind", PrimitiveType::CUBE, 10)
{
	delete m_GameObjectUI;
	m_GameObjectUI = new WindUI (name);

	windUI = dynamic_cast<WindUI*> (m_GameObjectUI);

	SetFullForceRadius (fullForceRadius);
	SetAttenuationRadius (attenuationRadius);
	SetForceMultiplier (forceMult);
}

void Wind::GenerateUI () { Primitive::GenerateUI(); }

void Wind::UpdateWithUI ()
{
	m_Transform.UpdateWithUI();
	m_Mesh.GetMaterial().UpdateWithUI();
	
	m_FullForceRadius = windUI->m_FullForceRadiusData;
	m_AttenuationRadius = windUI->m_AttenuationRadiusData;
	m_ForceMultiplier = windUI->m_ForceData;
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
