#pragma once
#include "glad/glad.h"
#include "../engine/Primitive.h"

class WindUI;

class Wind : public Primitive
{
public:
	Wind (GLfloat fullForceRadius = 5, GLfloat attenuationRadius = 20, GLfloat forceMult = 10);

	void GenerateUI() override;

	void Create() override {}
	void Update() override {}

	void UpdateWithUI() override;

	void SetFullForceRadius(GLfloat fullForceRadius); 
	void SetAttenuationRadius(GLfloat attenuationRadius); 
	void SetForceMultiplier(GLfloat forceMultiplier); 

private:
	GLfloat m_FullForceRadius;
	GLfloat m_AttenuationRadius;
	GLfloat m_ForceMultiplier;

	WindUI* windUI;
};
