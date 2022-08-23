#include "MassSpring.h"
#include "MassSpringUI.h"
#include "Wind.h"
#include "../engine/Scene.h"
#include "../engine/CollidingSphere.h"
#include "glm/gtx/transform.hpp"

MassSpring::MassSpring (const char* name, MassSpringParameters parameters) :
	GameObject (name),
	m_Parameters (std::move (parameters)) { m_GameObjectUI = new MassSpringUI (name); }

void MassSpring::Create ()
{
	SetupGraphicsShader (BlinnPhong);
	SetComputeBuffers();
	GenerateUI();
}

void MassSpring::Update ()
{
	// WIND
	auto wind = Scene::GetInstance()->GetWind();
	auto colliders = Scene::GetInstance()->GetColliders();
	auto inverseModel = glm::inverse (GetTransform().GetModelMatrix());
	if (wind != nullptr)
	{
		simulationStageComputeShader.Use();

		glm::vec4 forwardWind = glm::vec4 (0.0, 0.0, -1.0, 0.0);


		glm::mat4 rotationMatrix = glm::mat4 (1.0f);
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->alternativeRotation.x),
		                              wind->GetTransform().GetRightDirection());
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->alternativeRotation.y),
		                              wind->GetTransform().GetUpDirection());
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->alternativeRotation.z),
		                              wind->GetTransform().GetForwardDirection());

		//World forward of wind
		forwardWind = rotationMatrix * forwardWind;

		//Zw
		glm::vec3 localForwardWind = (glm::normalize (inverseModel * forwardWind));

		//Origin and axis in local space as vec4
		glm::vec4 localPositionWind4f = inverseModel * glm::vec4 (wind->GetTransform().GetPosition(), 1.0);
		glm::vec4 localForwardWind4f = glm::vec4 (localForwardWind, 0.0f);

		simulationStageComputeShader.SetUniform<glm::vec4>
			("wind.position", localPositionWind4f);

		simulationStageComputeShader.SetUniform<glm::vec4>
			("wind.forward", localForwardWind4f);

		simulationStageComputeShader.SetUniform<GLfloat>
			("wind.forceMult", wind->GetForceMultiplier());

		simulationStageComputeShader.SetUniform<GLfloat>
			("wind.fullForceRadius", wind->GetFullRadius());

		simulationStageComputeShader.SetUniform<GLfloat>
			("wind.attenuationRadius", wind->GetAttenuatedRadius());
	}

	// COLLIDING SPHERE
	if (!colliders.empty())
	{
		constraintsStageComputeShader.Use();
		for (int i = 0; i < colliders.size(); i++)
		{
			auto spherePos = 
			  inverseModel * glm::vec4 (colliders[i]->GetTransform().GetPosition(), 1);
		
			constraintsStageComputeShader.SetUniformArray<glm::vec4>
						  (("spheres[" + std::to_string(i) + "].sphereCenter").c_str(), spherePos);
			constraintsStageComputeShader.SetUniformArray<GLfloat>
							(("spheres[" + std::to_string(i) + "].sphereRadius").c_str(), colliders[i]->radius);
			constraintsStageComputeShader.SetUniformArray<GLuint>
							(("spheres[" + std::to_string(i) + "].sphereActive").c_str(), colliders[i]->m_IsActive);
		}
	}
}
