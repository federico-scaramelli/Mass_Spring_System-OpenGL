#pragma once
#include "glad/glad.h"
#include "../engine/Primitive.h"

class WindUI;

class Wind : public Primitive
{
public:
	glm::vec3 tiltedRotation {0,0,0};

	Wind (GLfloat fullForceRadius = 5, GLfloat attenuationRadius = 20, GLfloat forceMult = 10);

	void GenerateUI() override;

	void Create() override;
	void Update() override;

	void UpdateWithUI() override;

	void SetFullForceRadius(GLfloat fullForceRadius); 
	void SetAttenuationRadius(GLfloat attenuationRadius); 
	void SetForceMultiplier(GLfloat forceMultiplier);

	GLfloat GetForceMultiplier() { return m_ForceMultiplier; }
	GLfloat GetFullRadius() { return m_FullForceRadius; }
	GLfloat GetAttenuatedRadius() { return m_AttenuationRadius; }
	GLfloat GetReferenceDistance() { return m_ReferenceDistance; }

private:
	GLfloat m_TiltFactor = 5;

	// Radius in which the wind power remains constant
	GLfloat m_FullForceRadius;
	// Radius in which the wind power fades to 0.
	// It's defined as an external radius with respect to the full force radius.
	GLfloat m_AttenuationRadius;
	// Define the wind power
	GLfloat m_ForceMultiplier;
	// Define the reference distance to compute the wind cone from the two radiuses
	GLfloat m_ReferenceDistance;

	WindUI* windUI;
};
