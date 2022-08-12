#include "Material.h"



void Material::CreateShaderProgram(std::vector<std::pair<std::string, ShaderType>> pairList)
{
	for (auto& element : pairList)
	{
		m_Shader.CompileShader(element.first, element.second);
	}
	
	m_Shader.Link();
	m_Shader.Validate();
}