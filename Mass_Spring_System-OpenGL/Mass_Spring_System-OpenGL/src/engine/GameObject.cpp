#include "GameObject.h"

void GameObject::GenerateUI()
{
	m_Transform.GenerateUI(m_GameObjectUI->m_TransformUI);
	GetMaterial().GenerateUI (m_GameObjectUI->m_MaterialUI);
}

void GameObject::UpdateWithUI()
{
	m_Transform.UpdateWithUI();
	m_Mesh.GetMaterial().UpdateWithUI();
}

void GameObject::SetupGraphicsShader()
{
	m_Mesh.SetBuffers (vertexBufferLayout);
	m_Mesh.GetMaterial().CreateShaderProgram ( {
	  { "shader.vert", ShaderType::VERTEX },
		{ Material::fragShadersMap[m_Mesh.GetMaterial().fragShader], ShaderType::FRAGMENT }
	});		
}