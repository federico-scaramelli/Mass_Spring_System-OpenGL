#include "MassSpring.h"

#include "ClothPresets.h"
#include "MassSpringUI.h"
#include "Wind.h"
#include "../engine/Scene.h"
#include "../engine/CollidingSphere.h"
#include "glm/gtx/transform.hpp"

MassSpring::MassSpring (const char* name, MassSpringParameters parameters) :
	GameObject (name),
	m_Parameters (std::move (parameters))
{
	delete m_GameObjectUI;
	m_GameObjectUI = new MassSpringUI (name);

	m_MassSpringUI = dynamic_cast<MassSpringUI*> (m_GameObjectUI);

	m_MassSpringUI->m_StiffnessData = m_Parameters.stiffness;
	m_MassSpringUI->m_GravityData = m_Parameters.gravityAccel.y;
	m_MassSpringUI->m_ParticleMassData = m_Parameters.particleMass;
	m_MassSpringUI->m_SelfCollisionDistanceMultData = m_Parameters.selfCollisionDistanceMult;
	m_MassSpringUI->m_DragData = m_Parameters.drag;
}

// Constructor specifying a preset
MassSpring::MassSpring (const char* name, ClothPreset* preset) : GameObject (name),
                                                                 m_Parameters (preset->parameters), preset (preset)
{
	delete m_GameObjectUI;
	m_GameObjectUI = new MassSpringUI (name);

	m_MassSpringUI = dynamic_cast<MassSpringUI*> (m_GameObjectUI);

	m_MassSpringUI->m_StiffnessData = m_Parameters.stiffness;
	m_MassSpringUI->m_DragData = m_Parameters.drag;
	m_MassSpringUI->m_GravityData = m_Parameters.gravityAccel.y;
	m_MassSpringUI->m_ParticleMassData = m_Parameters.particleMass;
	m_MassSpringUI->m_ConstSheeringData = m_Parameters.kSheering;
	m_MassSpringUI->m_ConstBendingData = m_Parameters.kBending;
	m_MassSpringUI->m_CorrectionDumpingData = m_Parameters.constraintDistanceDumping;
	m_MassSpringUI->m_ConstraintDistanceMultData = m_Parameters.constraintDistanceMult;
	m_MassSpringUI->m_SelfCollisionDistanceMultData = m_Parameters.selfCollisionDistanceMult;
	m_MassSpringUI->m_SphereRepulsionDistMultData = m_Parameters.sphereRepulsionDistMult;
	m_MassSpringUI->m_SphereRepulsionDampingData = m_Parameters.sphereRepulsionDamping;
}

void MassSpring::Create ()
{
	SetupGraphicsShader (BlinnPhong);
	SetComputeBuffers();
	GenerateUI();

	// If a preset is specified, set the material UI with its values
	if (preset != nullptr && GetMaterial().fragShader == BlinnPhong)
	{
		dynamic_cast<BlinnPhongMaterialUI*> (GetUI().m_MaterialUI)->SetValues (
			preset->diffuseMat,
			preset->specularMat,
			preset->ambientMat,
			preset->shininessMat);
	}
}

void MassSpring::Update ()
{
	auto inverseModel = glm::inverse (GetTransform().GetModelMatrix());

	// Update uniforms related to wind
	auto wind = Scene::GetInstance()->GetWind();
	if (wind != nullptr)
	{
		simulationStageComputeShader.Use();

		glm::vec4 forwardWind = glm::vec4 (0.0, 0.0, -1.0, 0.0);


		glm::mat4 rotationMatrix = glm::mat4 (1.0f);
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->tiltedRotation.x),
		                              wind->GetTransform().GetRightDirection());
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->tiltedRotation.y),
		                              wind->GetTransform().GetUpDirection());
		rotationMatrix = glm::rotate (rotationMatrix,
		                              glm::radians (wind->tiltedRotation.z),
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

		simulationStageComputeShader.SetUniform<GLfloat>
			("wind.referenceDistance", wind->GetReferenceDistance());
	}

	// Update uniform related to the colliding spheres currently on the scene
	auto colliders = Scene::GetInstance()->GetColliders();
	if (!colliders.empty())
	{
		constraintsStageComputeShader.Use();

		for (auto it = colliders.begin();
		     //next_it = it;
		     it != colliders.end();)
		//it = next_it)
		{
			//++next_it;

			constraintsStageComputeShader.SetUniformArray<GLuint> ("sphereCount", colliders.size());

			auto collider = it->second;
			int i = std::distance (colliders.begin(), it);

			if (Scene::GetInstance()->GetGameObjects().find (it->first)
				!= Scene::GetInstance()->GetGameObjects().end())
			{
				auto spherePos =
					inverseModel * glm::vec4 (collider->GetTransform().GetPosition(), 1);

				constraintsStageComputeShader.SetUniformArray<glm::vec4>
					(("spheres[" + std::to_string (i) + "].sphereCenter").c_str(), spherePos);
				constraintsStageComputeShader.SetUniformArray<GLfloat>
					(("spheres[" + std::to_string (i) + "].sphereRadius").c_str(), collider->radius);
				constraintsStageComputeShader.SetUniformArray<GLuint>
					(("spheres[" + std::to_string (i) + "].sphereActive").c_str(), collider->m_IsActive);

				++it;
			}
			else
			{
				constraintsStageComputeShader.SetUniformArray<glm::vec4>
					(("spheres[" + std::to_string (i) + "].sphereCenter").c_str(), { 0, 0, 0, 0 });
				constraintsStageComputeShader.SetUniformArray<GLfloat>
					(("spheres[" + std::to_string (i) + "].sphereRadius").c_str(), 0);
				constraintsStageComputeShader.SetUniformArray<GLuint>
					(("spheres[" + std::to_string (i) + "].sphereActive").c_str(), 0);
				Scene::GetInstance()->EraseCollider (it->first);
				return;
				colliders.erase (it++);
				Scene::GetInstance()->GetColliders() = colliders;
			}
		}
	}
}

// Reset the cloth to its starting settings (pinned vertices only or complete preset)
void MassSpring::Reset ()
{
	auto& vertices = m_Mesh.GetVertices();
	std::vector<int> pinnedIdx;
	for (int i = 0; i < vertices.size(); i++) { if (vertices[i].pinned.x > 0.5f) { pinnedIdx.push_back (i); } }

	if (preset != nullptr)
	{
		GetTransform().SetPosition (preset->startingPos);
		GetTransform().SetRotation (preset->startingRot);
	}
	else
	{
		GetTransform().SetPosition ({ 0, 0, 0 });
		GetTransform().SetRotation ({ 0, 0, 0 });
	}

	InitializeVertices();
	for (int i : pinnedIdx) { vertices[i].pinned.x = 1.0; }
	GetMesh().UpdateBuffers();
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_Mesh.m_vbo.GetID());
	glBufferData (GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);
}

void MassSpring::UpdateWithUI ()
{
	GameObject::UpdateWithUI();

	m_Parameters.stiffness = m_MassSpringUI->m_StiffnessData;
	m_Parameters.drag = m_MassSpringUI->m_DragData;
	m_Parameters.particleMass = m_MassSpringUI->m_ParticleMassData;
	m_Parameters.kSheering = m_MassSpringUI->m_ConstSheeringData;
	m_Parameters.kBending = m_MassSpringUI->m_ConstBendingData;
	m_Parameters.gravityAccel.y = m_MassSpringUI->m_GravityData;

	m_Parameters.constraintDistanceDumping = m_MassSpringUI->m_CorrectionDumpingData;
	m_Parameters.constraintDistanceMult = m_MassSpringUI->m_ConstraintDistanceMultData;
	m_Parameters.selfCollisionDistanceMult = m_MassSpringUI->m_SelfCollisionDistanceMultData;
	m_Parameters.sphereRepulsionDistMult = m_MassSpringUI->m_SphereRepulsionDistMultData;
	m_Parameters.sphereRepulsionDamping = m_MassSpringUI->m_SphereRepulsionDampingData;
}
