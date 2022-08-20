#pragma once
#include <vector>
#include "MassSpring.h"

class PhysicsSolver
{
public:
	PhysicsSolver() = default;

	void SetActiveMassSpring (MassSpring* massSpring) { this->massSpring = massSpring; }
	void AddCollider (CollidingSphere* collider) { colliders.push_back (collider); }

	void Update()
	{
		if ( massSpring == nullptr || colliders.empty() ) return;

		UpdateCollidingSphereUniforms ();
	}

	std::vector<CollidingSphere*> colliders;

private:
	MassSpring* massSpring = nullptr;

	void UpdateCollidingSphereUniforms ()
	{
		if (!massSpring->constraintsStageComputeShader.IsLinked()) return;

		massSpring->constraintsStageComputeShader.Use();

		for (int i = 0; i < colliders.size(); i++)
		{
			auto spherePos = 
			  glm::inverse (massSpring->GetTransform().GetModelMatrix())
			  * glm::vec4 (colliders[i]->GetTransform().GetPosition(), 1);
			
  			massSpring->constraintsStageComputeShader.SetUniformArray<glm::vec4>
							(("spheres[" + std::to_string(i) + "].sphereCenter").c_str(), spherePos);
			massSpring->constraintsStageComputeShader.SetUniformArray<GLfloat>
							(("spheres[" + std::to_string(i) + "].sphereRadius").c_str(), colliders[i]->size);
			massSpring->constraintsStageComputeShader.SetUniformArray<GLuint>
							(("spheres[" + std::to_string(i) + "].sphereActive").c_str(), colliders[i]->m_IsActive);
		}
	}
};
