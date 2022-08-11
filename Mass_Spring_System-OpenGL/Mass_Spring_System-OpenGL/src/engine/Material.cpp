#include "Material.h"



void Material::CreateShader(std::vector<std::pair<std::string, ShaderType>> pairList)
{
	for (auto& element : pairList)
	{
		m_Shader.CompileShader(element.first, element.second);
	}

	// m_Shader.CompileShader("shader.vert", ShaderType::VERTEX);
	// m_Shader.CompileShader("shader.frag", ShaderType::FRAGMENT);
	
	m_Shader.Link();
	m_Shader.Validate();
}
