#pragma once
#include "../engine/GameObject.h"

class ClothPreset;

struct MassSpringParameters
{
	MassSpringParameters(const GLfloat dT, const uint16_t subSteps, 
						const GLfloat damping, const glm::vec4 gravityAcc, GLfloat particleMass, 
						GLfloat stiffness, GLfloat kSheering, GLfloat kBending,
						GLfloat constraintDistanceDumping, GLfloat constraintDistanceMult,
						GLfloat selfCollisionDistanceMult, GLfloat sphereRepulsionDistMult,
						GLfloat sphereRepulsionDamping) :

						deltaTime (dT), subSteps (subSteps),
						drag (damping), gravityAccel (gravityAcc), particleMass (particleMass),
						stiffness (stiffness), kSheering (kSheering), kBending(kBending),
						constraintDistanceDumping (constraintDistanceDumping), constraintDistanceMult (constraintDistanceMult),
						selfCollisionDistanceMult (selfCollisionDistanceMult), sphereRepulsionDistMult (sphereRepulsionDistMult),
						sphereRepulsionDamping (sphereRepulsionDamping)
	{
		subStepDt = dT / static_cast<float> (subSteps);
	}

	GLfloat deltaTime;
	uint16_t subSteps;
	GLfloat subStepDt;

	GLfloat drag;
	glm::vec4 gravityAccel;
	GLfloat particleMass;

	GLfloat stiffness;
	GLfloat kSheering;
	GLfloat kBending;

	GLfloat constraintDistanceDumping;
	GLfloat constraintDistanceMult;
	GLfloat selfCollisionDistanceMult;
	GLfloat sphereRepulsionDistMult;
	GLfloat sphereRepulsionDamping;
};

class MassSpringUI;

class MassSpring : public GameObject
{
protected:
	MassSpring (const char* name, MassSpringParameters parameters);
	MassSpring (const char* name, ClothPreset* preset);
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

	ClothPreset* preset;

	virtual void Reset ();

	void UpdateWithUI () override;
};
