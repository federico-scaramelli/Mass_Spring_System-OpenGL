#pragma once
#include <vector>
#include "MassSpring.h"
#include "../engine/SphereCollider.h"

class PhysicsSolver
{
public:
	PhysicsSolver() = default;

	void SetActiveMassSpring (MassSpring* massSpring) { this->massSpring = massSpring; }
	void SetActiveCollider (SphereCollider* collider) { this->collider = collider; }

	void Update()
	{
		if ( massSpring == nullptr || collider == nullptr ) return;

		UpdateCollidingSphereUniforms ();
	}

private:
	MassSpring* massSpring = nullptr;
	//std::vector<SphereCollider> colliders;
	SphereCollider* collider = nullptr;

	void UpdateCollidingSphereUniforms ()
	{
		auto spherePos = glm::inverse (massSpring->GetTransform().GetModelMatrix())
			* glm::vec4 (collider->centerPosition, 1);

		massSpring->constraintsStageComputeShader.Use();
		massSpring->constraintsStageComputeShader.SetUniform<glm::vec4> ("sphereCenter", spherePos);
		massSpring->constraintsStageComputeShader.SetUniform<GLfloat> ("sphereRadius", collider->size);
	}
};
