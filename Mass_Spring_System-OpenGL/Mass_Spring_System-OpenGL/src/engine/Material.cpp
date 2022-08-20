#include "Material.h"
#include "MaterialUI.h"

void Material::CreateShaderProgram(std::vector<std::pair<std::string, ShaderType>> pairList)
{
	for (auto& element : pairList)
	{
		m_Shader.CompileShader(element.first, element.second);
	}
	
	m_Shader.Link();
	m_Shader.Validate();
}

void Material::GenerateUI (MaterialUI* materialUI) { m_MaterialUI = materialUI; }

void Material::UpdateWithUI ()
{
	m_DiffuseColor = { m_MaterialUI->m_DiffuseData[0], m_MaterialUI->m_DiffuseData[1], m_MaterialUI->m_DiffuseData[2] };
	m_SpecularColor = { m_MaterialUI->m_SpecularData[0], m_MaterialUI->m_SpecularData[1], m_MaterialUI->m_SpecularData[2] };
	m_AmbientColor = { m_MaterialUI->m_AmbientData[0], m_MaterialUI->m_AmbientData[1], m_MaterialUI->m_AmbientData[2] };
	m_Shininess = m_MaterialUI->m_ShininessData;
}