#include "PhysicsSolver.h"

#include "../engine/CollidingSphere.h"
#include "Wind.h"
#include "glm/gtx/transform.hpp"

void PhysicsSolver::UpdateCollidingSphereUniforms ()
{
	if (!massSpring->constraintsStageComputeShader.IsLinked()) return;

	auto inverseModel = glm::inverse (massSpring->GetTransform().GetModelMatrix());
	
	massSpring->constraintsStageComputeShader.Use();

	for (int i = 0; i < colliders.size(); i++)
	{
		auto spherePos = inverseModel * glm::vec4 (colliders[i]->GetTransform().GetPosition(), 1);
		
  		massSpring->constraintsStageComputeShader.SetUniformArray<glm::vec4>
						(("spheres[" + std::to_string(i) + "].sphereCenter").c_str(), spherePos);
		massSpring->constraintsStageComputeShader.SetUniformArray<GLfloat>
						(("spheres[" + std::to_string(i) + "].sphereRadius").c_str(), colliders[i]->radius);
		massSpring->constraintsStageComputeShader.SetUniformArray<GLuint>
						(("spheres[" + std::to_string(i) + "].sphereActive").c_str(), colliders[i]->m_IsActive);
	}
}

void PhysicsSolver::UpdateWindUniforms()
{
	if (!massSpring->constraintsStageComputeShader.IsLinked()) return;
	
	auto inverseModel = glm::inverse (massSpring->GetTransform().GetModelMatrix());

	massSpring->simulationStageComputeShader.Use();

	glm::vec4 forwardWind = glm::vec4(0.0, 0.0, -1.0, 0.0);
	
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate (rotationMatrix, glm::radians (wind->alternativeRotation.x), wind->GetTransform().GetRightDirection());
	rotationMatrix = glm::rotate (rotationMatrix, glm::radians (wind->alternativeRotation.y), wind->GetTransform().GetUpDirection());
	rotationMatrix = glm::rotate (rotationMatrix, glm::radians (wind->alternativeRotation.z), wind->GetTransform().GetForwardDirection());

	//World forward of wind
	forwardWind = rotationMatrix * forwardWind;

	//Zw
	glm::vec3 localForwardWind = (glm::normalize( inverseModel * forwardWind ));
	//Xw
	glm::vec3 localRigthtWind = glm::normalize( glm::cross( localForwardWind, Transform::worldUpDirection ));
	//Yw
	glm::vec3 localUptWind = glm::normalize( glm::cross( localRigthtWind, localForwardWind ));

	//Origin and axis in local space as vec4
	glm::vec4 localPositionWind4f = inverseModel * glm::vec4(wind->GetTransform().GetPosition(), 1.0);
	glm::vec4 localForwardWind4f = glm::vec4(localForwardWind, 0.0f);
	glm::vec4 localRightWind4f = glm::vec4(localRigthtWind, 0.0f);
	glm::vec4 localUpdWind4f = glm::vec4(localUptWind, 0.0f);


	massSpring->simulationStageComputeShader.SetUniform<glm::vec4>
		("wind.position", localPositionWind4f);

	massSpring->simulationStageComputeShader.SetUniform<glm::vec4>
		("wind.forward", localForwardWind4f);

	massSpring->simulationStageComputeShader.SetUniform<glm::vec4>
		("wind.right", localRightWind4f);

	massSpring->simulationStageComputeShader.SetUniform<glm::vec4>
		("wind.up", localUpdWind4f);

	massSpring->simulationStageComputeShader.SetUniform<GLfloat>
		("wind.forceMult", wind->GetForceMultiplier());

	massSpring->simulationStageComputeShader.SetUniform<GLfloat>
		("wind.fullForceRadius", wind->GetFullRadius());

	massSpring->simulationStageComputeShader.SetUniform<GLfloat>
		("wind.attenuationRadius", wind->GetAttenuatedRadius());

}
