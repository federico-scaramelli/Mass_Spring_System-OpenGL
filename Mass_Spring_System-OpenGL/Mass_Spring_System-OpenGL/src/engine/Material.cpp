#include "Material.h"

#include "BlinnPhongMaterialUI.h"
#include "Camera.h"
#include "LightSource.h"
#include "MaterialUI.h"
#include "Scene.h"
#include "SolidMaterialUI.h"

Material::~Material ()
{
	delete materialParams;
}

void Material::Setup (FragmentShader fragmentPreset)
{
	fragShader = fragmentPreset;
	CreateShaderProgram ({
		{ "shader.vert", ShaderType::VERTEX },
		{ fragShadersMap[fragShader], ShaderType::FRAGMENT }
	});

	switch (fragShader)
	{
	case BlinnPhong:
		materialParams = new BlinnPhongParameters();
		break;
	case Solid:
		materialParams = new SolidParameters();
		break;
	}
}

void Material::Update ()
{
	Scene* scene = Scene::GetInstance();
	m_Shader.Use();

	switch (fragShader)
	{
	case BlinnPhong:
		if (scene->GetLightSource() != nullptr)
		{
			auto params = dynamic_cast<BlinnPhongParameters*>(materialParams);
			m_Shader.SetUniform<glm::vec3> ("lightPosition",
											scene->GetLightSource()->GetTransform().GetPosition());

			m_Shader.SetUniform<glm::vec3> ("lightAmbient",
											scene->GetLightSource()->GetAmbient());
			m_Shader.SetUniform<glm::vec3> ("lightDiffuse",
											scene->GetLightSource()->GetIntensity());

			m_Shader.SetUniform<glm::vec3> ("matAmbient", params->ambientColor);
			m_Shader.SetUniform<glm::vec3> ("matDiffuse",params->diffuseColor);
			m_Shader.SetUniform<glm::vec3> ("matSpecular", params->specularColor);
			m_Shader.SetUniform<GLfloat> ("matShininess", params->shininess);
		}
		break;
	case Solid:
		auto params = dynamic_cast<SolidParameters*>(materialParams);
		m_Shader.SetUniform<glm::vec3> ("solidColor", params->color);
		break;
	}
	
}

void Material::CreateShaderProgram (std::vector<std::pair<std::string, ShaderType>> pairList)
{
	for (auto& element : pairList) { m_Shader.CompileShader (element.first, element.second); }

	m_Shader.Link();
	m_Shader.Validate();
	m_Shader.Setup();
}

void Material::GenerateUI (GameObject* gameObject)
{
	switch (fragShader)
	{
	case BlinnPhong:
		m_MaterialUI = new BlinnPhongMaterialUI(gameObject->name.c_str());
		break;
	case Solid:
		m_MaterialUI = new SolidMaterialUI(gameObject->name.c_str());
		break;
	default:
	  return;
	}
	gameObject->GetUI().m_MaterialUI = m_MaterialUI;
}

void Material::UpdateWithUI ()
{
	materialParams->UpdateWithUI (m_MaterialUI);
}
