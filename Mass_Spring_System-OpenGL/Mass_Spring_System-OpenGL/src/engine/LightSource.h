#pragma once
#include "LightUI.h"
#include "Primitive.h"

class LightSource : public Primitive
{
private:
	// Both diffuse and specular color
	glm::vec3 m_LightIntensity {1, 1, .5f};
	glm::vec3 m_LightAmbient {0, 0, 0};

public:
	// Create a light representing it as a Solid Sphere on the scene
	LightSource(const GLfloat size = 5, glm::vec3 lightIntensity = {1,1,1}, glm::vec3 lightAmbient = {1,1,1})
	  : Primitive ("Light", PrimitiveType::SPHERE, size),
		m_LightIntensity (lightIntensity), m_LightAmbient (lightAmbient)
	{
		delete m_GameObjectUI;
		// Instantiate the custom UI
		m_GameObjectUI = new LightUI(name);
	}

	void GenerateUI() override
	{
		Primitive::GenerateUI();
	}

	void Create() override
	{
		SetupGraphicsShader (Solid);
		GenerateUI();
	}

	// No custom update behavior
	void Update() override {}

	void UpdateWithUI() override
	{
		m_Transform.UpdatePositionWithUI();
		m_Mesh.GetMaterial().UpdateWithUI();

		auto lightUI = dynamic_cast<LightUI*>(m_GameObjectUI);
		m_LightIntensity = {lightUI->m_LightDiffuseData[0], lightUI->m_LightDiffuseData[1], lightUI->m_LightDiffuseData[2]};
		m_LightAmbient = {lightUI->m_LightAmbientData[0], lightUI->m_LightAmbientData[1], lightUI->m_LightAmbientData[2]};
	}
		
	const glm::vec3& GetIntensity() const { return m_LightIntensity; } 
	const glm::vec3& GetAmbient() const { return m_LightAmbient; }
};