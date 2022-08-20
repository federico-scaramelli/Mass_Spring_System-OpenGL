#include "PhysicsSolver.h"

#include "../engine/CollidingSphere.h"

void PhysicsSolver::UpdateCollidingSphereUniforms ()
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