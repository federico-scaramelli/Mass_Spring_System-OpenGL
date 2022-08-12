#pragma once
#include "GameObject.h"
#include "Transform.h"

class LightSource : public GameObject
{
private:
	glm::vec3 m_LightIntensity;
	glm::vec3 m_LightAmbient;
	
	float UI_LightIntensity[3] = {1, 1, 1};
	float UI_LightAmbient[3] = {1, 1, 1};

	char UILabel_Intensity[50];
	char UILabel_Ambient[50];

public:
  LightSource(glm::vec3 lightIntensity = {1,1,1}, glm::vec3 lightAmbient = {1,1,1}) : GameObject ("Light ")
	{
		this->m_LightIntensity = lightIntensity;
		this->m_LightAmbient = lightAmbient;
	}

	void GenerateUI(Renderer& renderer) override
	{
		m_Transform.GeneratePositionUI (renderer);
		GenerateLightUI (renderer);
	}

	void GenerateLightUI (Renderer& renderer, float min = 0.f, float max = 1.f)
	{
		strcpy_s (UILabel_Intensity, name);
		strcat_s (UILabel_Intensity, " intensity");
		renderer.AddFloat3SliderUI (UILabel_Intensity, UI_LightIntensity, min, max);
		
		strcpy_s (UILabel_Ambient, name);
		strcat_s (UILabel_Ambient, " ambient");
		renderer.AddFloat3SliderUI (UILabel_Ambient, UI_LightAmbient, min, max);
	}

	void UpdateWithUI() override
	{
		m_Transform.UpdatePositionWithUI();
		m_LightIntensity = {UI_LightIntensity[0], UI_LightIntensity[1], UI_LightIntensity[2]};
		m_LightAmbient = {UI_LightAmbient[0], UI_LightAmbient[1], UI_LightAmbient[2]};
	}
		
	const glm::vec3& GetIntensity() const { return m_LightIntensity; } 
	const glm::vec3& GetAmbient() const { return m_LightAmbient; }
};