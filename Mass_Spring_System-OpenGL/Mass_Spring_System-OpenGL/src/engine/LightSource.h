#pragma once
#include "GameObject.h"
#include "Transform.h"

class LightSource : public GameObject
{
private:
	glm::vec3 lightIntensity;

public:
	LightSource(glm::vec3 lightIntensity)
	{
		this->lightIntensity = lightIntensity;
	}
	
	const glm::vec3& GetIntensity() const { return lightIntensity; } 
};

