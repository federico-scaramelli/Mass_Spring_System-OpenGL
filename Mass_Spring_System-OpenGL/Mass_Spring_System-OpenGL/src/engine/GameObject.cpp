#include "GameObject.h"

void GameObject::GenerateUI()
{
	m_Transform.SetUIObject(m_GameObjectUI->m_TransformUI);
	GetMaterial().GenerateUI (m_GameObjectUI);
}

void GameObject::UpdateWithUI()
{
	m_Transform.UpdateWithUI();
	if (m_GameObjectUI->m_MaterialUI != nullptr)
	  m_Mesh.GetMaterial().UpdateWithUI();
}

void GameObject::SetupGraphicsShader(FragmentShader fragmentPreset)
{
	m_Mesh.SetBuffers (vertexBufferLayout);
	GetMaterial().Setup (fragmentPreset);
}