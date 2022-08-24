#pragma once
#include "../engine/GameObject.h"

struct MassSpringParameters
{
	MassSpringParameters(const GLfloat dT, const uint16_t subSteps, 
						const GLfloat damping, const glm::vec4 gravityAcc, GLfloat particleMass, 
						GLfloat stiffness, GLfloat kSheering, GLfloat kBending) :
						deltaTime (dT), subSteps (subSteps),
						damping (damping), gravityAccel (gravityAcc), particleMass (particleMass),
						stiffness (stiffness), kSheering (kSheering), kBending(kBending)
	{
		subStepDt = dT / static_cast<float> (subSteps);
	}

	GLfloat deltaTime;
	uint16_t subSteps;
	GLfloat subStepDt;

	GLfloat damping;
	glm::vec4 gravityAccel;
	GLfloat particleMass;

	GLfloat stiffness;
	GLfloat kSheering;
	GLfloat kBending;

	GLfloat correctionDumping = 0.25f;
	GLfloat constraintDistanceMult = 1.1f;
	GLfloat selfCollisionDistanceMult = 0.8f;
	GLfloat sphereRepulsionDistMult = 1.05f;
	GLfloat sphereRepulsionDamping = 0.95f;
};

class MassSpringUI;

class MassSpring : public GameObject
{
protected:
	MassSpring (const char* name, MassSpringParameters parameters);
	MassSpringParameters m_Parameters;

	MassSpringUI* m_MassSpringUI;

	virtual void InitializeVertices () = 0;
	virtual void InitializeIndices () = 0;

	virtual void BindComputeBuffers (int vboBind, int tempBind) = 0;

public:
	ShaderProgramCompute simulationStageComputeShader {};
	ShaderProgramCompute constraintsStageComputeShader {};

	virtual void SetComputeBuffers () = 0;
	void Create () override;
	void Update () override = 0;

	void UpdateWithUI () override;
};
