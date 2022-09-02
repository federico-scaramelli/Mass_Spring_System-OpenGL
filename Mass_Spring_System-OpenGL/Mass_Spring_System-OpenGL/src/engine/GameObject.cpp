#include "GameObject.h"

// Generate the transform and material UI
// m_GameObjectUI is created on derived classes constructor
void GameObject::GenerateUI()
{
	m_Transform.SetUIObject(m_GameObjectUI->m_TransformUI);
	GetMaterial().GenerateUI (this);
}

// Update transform and material properties with value on UI
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