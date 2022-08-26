#include "Rope.h"

#include "MassSpringUI.h"
#include "PhysicsParameters.h"
#include "glm/gtx/rotate_vector.hpp"

#include "../engine/Scene.h"
#include "../engine/CollidingSphere.h"
#include "glm/gtx/transform.hpp"
#include "Wind.h"


Rope::Rope(GLuint pointsByLength, uint16_t restLenght, GLfloat radius, MassSpringParameters parameters) :
	MassSpring ("Rope", parameters),
	m_RestLength (restLenght), m_PointsByLength (pointsByLength),
	m_Radius (radius)
{

	InitializeNodes();
	InitializeVertices();
	InitializeIndices();
}

void Rope::InitializeNodes()
{
	m_Nodes.clear();

	for (auto i = 0; i < m_PointsByLength; i++)
	{
		glm::vec3 initialPosition
		{
			i * m_RestLength,
			0.0f,
			0.0f
		};

		Node node = {{initialPosition, 1.0f}};
		node.currentUp = {0,1,0,0};
		m_Nodes.push_back(node);
	}

	m_Nodes[0].pinned = {1,0,0,0};
}

void Rope::InitializeVertices()
{
	auto& vertices = m_Mesh.GetVertices();
	vertices.clear();

	glm::vec3 nextNodeForwardDir = {1, 0, 0};
	glm::vec3 nextNodeUpDir = {0, 1, 0};
	glm::vec3 nextNodeRightDir = {0, 0, 1};

	//Create vertices for nodes except last one (last one gets last axis, not updated)
	for (auto i = 0; i < m_Nodes.size(); i++)
	{
		//Update axis for all nodes except last node, with next node position
		if (i < m_Nodes.size() - 1)
		{
			nextNodeForwardDir = glm::normalize(m_Nodes[i + 1].position - m_Nodes[i].position);
			nextNodeRightDir = glm::normalize(glm::cross(nextNodeForwardDir, Transform::worldUpDirection));
			nextNodeUpDir = glm::normalize(glm::cross(nextNodeRightDir, nextNodeForwardDir));
		}

		for (auto weigth : vertexPositionsWeigths)
		{
			glm::vec3 nodePos = glm::vec3(m_Nodes[i].position);

			glm::vec3 positionSide = nextNodeRightDir * weigth.x * m_Radius;
			glm::vec3 positionFront = nextNodeUpDir * weigth.y * m_Radius;

			glm::vec3 position = positionSide + positionFront;
			
			position += glm::vec3(m_Nodes[i].position);

			glm::vec3 normal = glm::normalize(position - nodePos);

			Vertex vertex{{position.x, position.y, position.z, 0.0f}};
			
			if(i == 0)
			{
				auto dirToNext = glm::normalize(glm::vec3(m_Nodes[i+1].position) - nodePos);
				normal += dirToNext;	
			}
			if(i == m_Nodes.size() - 1)
			{
				auto dirFromPrec = glm::normalize(nodePos - glm::vec3(m_Nodes[i-1].position));
				normal += dirFromPrec;	
			}

			vertex.normal = {normal, 0.0f};

			vertices.push_back(vertex);
		}
	}
}

void Rope::InitializeIndices()
{
	CreateFrontSurfaceIndices();
	CreateMidSurfaceIndices();
	CreateBackSurfaceIndices();
}

void Rope::CreateFrontSurfaceIndices()
{
	auto& indices = m_Mesh.GetIndices();

	indices.clear();

	int maxindexOffset = vertexPositionsWeigths.size() - 1;
	int i = 0;
	while (((maxindexOffset - i - 1) - i) > 1)
	{
		indices.push_back(i);
		indices.push_back(maxindexOffset - i - 1);
		indices.push_back(maxindexOffset - i);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(maxindexOffset - i - 1);

		i++;
	}
}

void Rope::CreateMidSurfaceIndices()
{
	auto& indices = m_Mesh.GetIndices();

	int numberOfIndices = vertexPositionsWeigths.size();
	for (int i = 0; i < m_Nodes.size() - 1; i++)
	{
		int current = i * numberOfIndices; //0
		int lastIndexCurrentCycle = current + (numberOfIndices - 1); //7
		int next = (i + 1) * numberOfIndices; //8
		int lastIndexNextCycle = next + (numberOfIndices - 1); //15

		for (int j = 0; j < numberOfIndices - 1; j++)
		{
			indices.push_back(current + j);
			indices.push_back(current + j + numberOfIndices);
			indices.push_back(current + j + 1);

			indices.push_back(current + j + numberOfIndices);
			indices.push_back(current + j + 1 + numberOfIndices);
			indices.push_back(current + j + 1);
		}

		indices.push_back(lastIndexCurrentCycle);
		indices.push_back(lastIndexNextCycle);
		indices.push_back(current);

		indices.push_back(lastIndexNextCycle);
		indices.push_back(next);
		indices.push_back(current);

		// auto x=2;
	}
}

void Rope::CreateBackSurfaceIndices()
{
	auto& indices = m_Mesh.GetIndices();

	int maxIndexOffset = vertexPositionsWeigths.size() - 1; //7

	int startIndex = (m_Nodes.size() - 1) * (maxIndexOffset + 1); //72
	int i = 0;

	while (((maxIndexOffset - i - 1) - i) > 1)
	{
		indices.push_back(startIndex + i);
		indices.push_back(startIndex + maxIndexOffset - i);
		indices.push_back(startIndex + maxIndexOffset - i - 1);

		indices.push_back(startIndex + i);
		indices.push_back(startIndex + maxIndexOffset - i - 1);
		indices.push_back(startIndex + i + 1);

		i++;
	}
}

void Rope::SetComputeBuffers()
{
	// Compute stage 1: compute new positions without constraints
	simulationStageComputeShader.CreateProgram ({ "eulerConstraintsRopeShader.comp", ShaderType::COMPUTE });
	simulationStageComputeShader.SetWorkGroupSize ({ 16, 1, 1 });
	simulationStageComputeShader.SetWorkGroupNum ({ m_Nodes.size(), 1, 1 });

	// Compute stage 2: apply constraints
	constraintsStageComputeShader.CreateProgram ({ "ropeVertices.comp", ShaderType::COMPUTE });
	constraintsStageComputeShader.SetWorkGroupSize ({ 16, 1, 1 });
	constraintsStageComputeShader.SetWorkGroupNum ({ GetMesh().GetVertices().size(), 1, 1 });

	glGenBuffers (1, &m_ComputeNodesInBuffer);
	glGenBuffers (1, &m_ComputeNodesOutBuffer);

	auto size = sizeof(Node) * m_Nodes.size();

	simulationStageComputeShader.Use();
	//In
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_ComputeNodesInBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, size, m_Nodes.data(), GL_DYNAMIC_DRAW);

	// Out
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_ComputeNodesOutBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, size, m_Nodes.data(), GL_DYNAMIC_DRAW);

	constraintsStageComputeShader.Use();
	
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, m_Mesh.m_vbo.GetID());
	glBufferData (GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);

}

void Rope::Create()
{
	MassSpring::Create();

	simulationStageComputeShader.Use();

	simulationStageComputeShader.SetUniform<GLuint> ("ropeDim", m_PointsByLength);

	simulationStageComputeShader.SetUniform<GLfloat> ("radius", m_Radius);

	simulationStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLength);

	simulationStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);

	simulationStageComputeShader.SetUniform<GLfloat> ("damping", m_Parameters.damping);
	
	simulationStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);

	simulationStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);

	simulationStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);

	simulationStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);

	simulationStageComputeShader.SetUniform<glm::vec4> ("gravityAcceleration", glm::inverse(GetTransform().GetUpdatedModelMatrix()) * m_Parameters.gravityAccel);

	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.correctionDumping", m_Parameters.constraintDistanceDumping);

	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.constraintDistanceMult", m_Parameters.constraintDistanceMult);

	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.selfCollisionDistanceMult", m_Parameters.selfCollisionDistanceMult);

	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDistMult", m_Parameters.sphereRepulsionDistMult);

	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDamping", m_Parameters.sphereRepulsionDamping);

	constraintsStageComputeShader.Use();
	
	constraintsStageComputeShader.SetUniform<GLuint> ("verticesDim", m_Mesh.GetVertices().size());

	constraintsStageComputeShader.SetUniform<GLuint> ("ropeDim", m_PointsByLength);

	constraintsStageComputeShader.SetUniform<GLfloat> ("radius", m_Radius);
}

void Rope::Update()
{
	// MassSpring::Update();

	static int readBuf = 0;

	if (Physics::deltaTime >= 1.0)
	{
		
		for (int i = 0; i < m_Parameters.subSteps; i++)
		{
			simulationStageComputeShader.Use();
			glBindBufferBase (GL_SHADER_STORAGE_BUFFER, readBuf, m_ComputeNodesInBuffer);
			glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1-readBuf, m_ComputeNodesOutBuffer);
			simulationStageComputeShader.Compute();
			simulationStageComputeShader.Wait();
			
			constraintsStageComputeShader.Use();
			glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1-readBuf, m_ComputeNodesInBuffer);
			glBindBufferBase (GL_SHADER_STORAGE_BUFFER, readBuf, m_ComputeNodesOutBuffer);
			glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, m_Mesh.m_vbo.GetID());
			constraintsStageComputeShader.Compute();
			constraintsStageComputeShader.Wait();

			readBuf = 1 - readBuf;

			// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, readBuf, m_ComputeNodesInBuffer);
			// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1-readBuf, m_ComputeNodesOutBuffer);
			// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, m_Mesh.m_vbo.GetID());
		}

		Physics::deltaTime--;
	}

	simulationStageComputeShader.Use();

	simulationStageComputeShader.SetUniform<glm::vec4> ("gravityAcceleration", glm::inverse(GetTransform().GetUpdatedModelMatrix()) * m_Parameters.gravityAccel);

	simulationStageComputeShader.SetUniform<GLfloat> ("damping", m_Parameters.damping);
	simulationStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);
	simulationStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);
	simulationStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);
	simulationStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.correctionDumping", m_Parameters.constraintDistanceDumping);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.constraintDistanceMult", m_Parameters.constraintDistanceMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.selfCollisionDistanceMult", m_Parameters.selfCollisionDistanceMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDistMult", m_Parameters.sphereRepulsionDistMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDamping", m_Parameters.sphereRepulsionDamping);

	auto inverseModel = glm::inverse(GetTransform().GetModelMatrix());

	simulationStageComputeShader.SetUniform<glm::vec4>("gravityAcceleration",
	                                                   glm::inverse(GetTransform().GetUpdatedModelMatrix()) *
	                                                   m_Parameters.gravityAccel);

	simulationStageComputeShader.SetUniform<GLfloat>("damping", m_Parameters.damping);
	simulationStageComputeShader.SetUniform<GLfloat>("elasticStiffness", m_Parameters.stiffness);
	simulationStageComputeShader.SetUniform<GLfloat>("particleMass", m_Parameters.particleMass);
	simulationStageComputeShader.SetUniform<GLfloat>("constShearMult", m_Parameters.kSheering);
	simulationStageComputeShader.SetUniform<GLfloat>("constBendMult", m_Parameters.kBending);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.correctionDumping",
	                                                 m_Parameters.constraintDistanceDumping);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.constraintDistanceMult",
	                                                 m_Parameters.constraintDistanceMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.selfCollisionDistanceMult",
	                                                 m_Parameters.selfCollisionDistanceMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDistMult",
	                                                 m_Parameters.sphereRepulsionDistMult);
	simulationStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDamping",
	                                                 m_Parameters.sphereRepulsionDamping);

	// WIND
  auto wind = Scene::GetInstance()->GetWind();
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

	simulationStageComputeShader.SetUniform<GLfloat>
      ("wind.referenceDistance", wind->GetReferenceDistance());
  }

	// COLLIDING SPHERE
	auto colliders = Scene::GetInstance()->GetColliders();
	if (!colliders.empty())
	{
		simulationStageComputeShader.Use();

		for (auto it = colliders.begin();
		     //next_it = it;
		     it != colliders.end();)
		//it = next_it)
		{
			//++next_it;

			simulationStageComputeShader.SetUniformArray<GLuint>("sphereCount", colliders.size());

			auto collider = it->second;
			int i = std::distance(colliders.begin(), it);

			if (Scene::GetInstance()->GetGameObjects().find(it->first)
				!= Scene::GetInstance()->GetGameObjects().end())
			{
				auto spherePos =
					inverseModel * glm::vec4(collider->GetTransform().GetPosition(), 1);

				simulationStageComputeShader.SetUniformArray<glm::vec4>
					(("spheres[" + std::to_string(i) + "].sphereCenter").c_str(), spherePos);
				simulationStageComputeShader.SetUniformArray<GLfloat>
					(("spheres[" + std::to_string(i) + "].sphereRadius").c_str(), collider->radius);
				simulationStageComputeShader.SetUniformArray<GLuint>
					(("spheres[" + std::to_string(i) + "].sphereActive").c_str(), collider->m_IsActive);

				++it;
			}
			else
			{
				simulationStageComputeShader.SetUniformArray<glm::vec4>
					(("spheres[" + std::to_string(i) + "].sphereCenter").c_str(), {0, 0, 0, 0});
				simulationStageComputeShader.SetUniformArray<GLfloat>
					(("spheres[" + std::to_string(i) + "].sphereRadius").c_str(), 0);
				simulationStageComputeShader.SetUniformArray<GLuint>
					(("spheres[" + std::to_string(i) + "].sphereActive").c_str(), 0);
				Scene::GetInstance()->EraseCollider(it->first);
				return;
				colliders.erase(it++);
				Scene::GetInstance()->GetColliders() = colliders;
			}
		}
	}
}

void Rope::Reset ()
{
	MassSpring::Reset();
	auto size = sizeof(Node) * m_Nodes.size();
	//In
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_ComputeNodesInBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, size, m_Nodes.data(), GL_DYNAMIC_DRAW);

	// Out
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_ComputeNodesOutBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, size, m_Nodes.data(), GL_DYNAMIC_DRAW);
}
