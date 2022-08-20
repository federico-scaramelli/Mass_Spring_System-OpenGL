#pragma once

#include "ShaderProgramCompute.h"

#include <string>
#include <vector>
#include "glm/vec3.hpp"

class MaterialUI;
class ShaderProgram;

class Material
{
private:
	ShaderProgram m_Shader{};

	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;
	glm::vec3 m_AmbientColor;
	float m_Shininess;

	MaterialUI* m_MaterialUI;

public:
	Material (glm::vec3 diffuseColor = { 0.5f, 0.5f, 0.5f },
	          glm::vec3 specularColor = { 0.5f, 0.5f, 0.5f },
	          glm::vec3 ambientColor = { 0.5f, 0.5f, 0.5f },
	          float shininess = 1) :
		m_DiffuseColor (diffuseColor), m_SpecularColor (specularColor), m_AmbientColor (ambientColor),
		m_Shininess (shininess) { }

	void SetProperties (glm::vec3 diffuseColor,
	                    glm::vec3 specularColor,
	                    glm::vec3 ambientColor,
	                    float shininess)
	{
		m_DiffuseColor = diffuseColor;
		m_SpecularColor = specularColor;
		m_AmbientColor = ambientColor;
		m_Shininess = shininess;
	}

	void CreateShaderProgram (std::vector<std::pair<std::string, ShaderType>> pairList);

	ShaderProgram& GetShader () { return m_Shader; }

	glm::vec3 GetAmbient () { return m_AmbientColor; }
	glm::vec3 GetDiffuse () { return m_DiffuseColor; }
	glm::vec3 GetSpecular () { return m_SpecularColor; }
	float GetShininess () { return m_Shininess; }

	void GenerateUI (MaterialUI* materialUI);
	void UpdateWithUI ();
};
