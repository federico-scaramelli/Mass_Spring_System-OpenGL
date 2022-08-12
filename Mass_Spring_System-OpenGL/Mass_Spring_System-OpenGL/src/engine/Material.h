#pragma once

#include "ShaderProgram.h"

#include <string>
#include <vector>
#include "glm/vec3.hpp"

class ShaderProgram;

class Material {
public:
	// glm::vec3 m_SurfaceColor{0.15f, 0.15f, 0.7f};

	glm::vec3 m_DiffuseColor;
	glm::vec3 m_SpecularColor;
	glm::vec3 m_AmbientColor;

	float m_Shininess;

	Material(glm::vec3 diffuseColor = {0.15f, 0.15f, 0.7f}, 
			 glm::vec3 specularColor = {1.f, 1.f, 1.f}, 
			 glm::vec3 ambientColor = {0.15f, 0.15f, 0.7f}, 
			 float shininess = 1) :
	m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor), m_AmbientColor(ambientColor), m_Shininess(shininess) { }


	void CreateShaderProgram(std::vector<std::pair<std::string, ShaderType>> pairList);

	ShaderProgram& GetShader() { return m_Shader; }

private:
	ShaderProgram m_Shader{};
};

