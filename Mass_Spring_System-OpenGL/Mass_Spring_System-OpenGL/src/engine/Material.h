#pragma once

#include "ShaderProgramCompute.h"

#include <string>
#include <vector>

#include "BlinnPhongMaterialUI.h"
#include "GameObjectUI.h"
#include "SolidMaterialUI.h"
#include "glm/vec3.hpp"

class SolidMaterialUI;
class BlinnPhongMaterialUI;
class MaterialUI;
class ShaderProgram;

enum FragmentShader
{
	BlinnPhong,
	Normal,
	Solid,
	Velocity
};

struct MaterialParameters
{
	virtual void UpdateWithUI(MaterialUI* matUI) = 0;
};

struct BlinnPhongParameters : MaterialParameters
{
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColor;
	float shininess;

	void UpdateWithUI(MaterialUI* matUI)
	{
		auto UI = dynamic_cast<BlinnPhongMaterialUI*>(matUI);
		diffuseColor = {UI->m_DiffuseData[0], UI->m_DiffuseData[1], UI->m_DiffuseData[2]};
		specularColor = {UI->m_SpecularData[0], UI->m_SpecularData[1], UI->m_SpecularData[2]};
		ambientColor = {UI->m_AmbientData[0], UI->m_AmbientData[1], UI->m_AmbientData[2]};
		shininess = UI->m_ShininessData;
	}
};

struct SolidParameters : MaterialParameters
{
	glm::vec3 color;

	void UpdateWithUI(MaterialUI* matUI)
	{
		auto UI = dynamic_cast<SolidMaterialUI*>(matUI);
		color = {UI->m_ColorData[0], UI->m_ColorData[1], UI->m_ColorData[2]};
	}
};

class Material
{
private:
	ShaderProgram m_Shader{};
	
	MaterialParameters* materialParams;
	
	MaterialUI* m_MaterialUI;

public:
	Material () = default;
	~Material ();

	void Setup(FragmentShader fragmentPreset);
	void Update();
	void CreateShaderProgram (std::vector<std::pair<std::string, ShaderType>> pairList);

	void GenerateUI (GameObjectUI* gameObjectUI);
	void UpdateWithUI ();

	ShaderProgram& GetShader () { return m_Shader; }
	MaterialParameters* GetParameters () { return materialParams; }

	inline static std::unordered_map<FragmentShader, const char*> fragShadersMap =
	{
		{BlinnPhong, "blinnPhongShader.frag"},
		{Normal, "normalShader.frag"},
		{Solid, "solidShader.frag"},
		{Velocity, "velocityShader.frag"}
	};
	FragmentShader fragShader = BlinnPhong;
};
