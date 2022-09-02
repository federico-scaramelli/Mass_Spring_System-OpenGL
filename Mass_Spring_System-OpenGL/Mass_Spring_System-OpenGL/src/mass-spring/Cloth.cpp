#include "Cloth.h"
#include "Cloth.h"

#include <iostream>
#include <random>
#include "../engine/Vertex.h"
#include "MassSpringUI.h"
#include "PhysicsParameters.h"
#include "../engine/Scene.h"
#include "ClothPresets.h"

// Standard constructor
Cloth::Cloth (const char* name, uint16_t pointsByWidth, uint16_t pointsByHeight, float restLenghtHV, MassSpringParameters parameters) :
	MassSpring (name, parameters),
	m_PointsByWidth (pointsByWidth), m_PointsByHeight (pointsByHeight),
	m_RestLengthHV (restLenghtHV)
{
	InitializeVertices();
	InitializeIndices();

	m_RestLengthDiagonal = static_cast<GLfloat> (sqrt (pow (m_RestLengthHV, 2) * 2));
}

// Preset constructor
Cloth::Cloth (const char* name, ClothPreset* preset): MassSpring (name, preset)
{
	m_PointsByWidth = preset->pointsByWidth;
	m_PointsByHeight = preset->pointsByHeight;
	m_RestLengthHV = preset->restLenghtHV;
	InitializeVertices();
	InitializeIndices();
	m_RestLengthDiagonal = static_cast<GLfloat> (sqrt (pow (m_RestLengthHV, 2) * 2));
}

// Create vertices procedurally
void Cloth::InitializeVertices ()
{
	auto& vertices = m_Mesh.GetVertices();

	vertices.clear();

	for (auto row = 0; row < m_PointsByHeight; row++)
	{
		for (auto column = 0; column < m_PointsByWidth; column++)
		{
			glm::vec3 initialPosition{
				column * m_RestLengthHV,
				row * m_RestLengthHV,
				0

			};

			Vertex vertex{ { initialPosition.x - m_RestLengthHV * m_PointsByWidth/2, initialPosition.y - m_RestLengthHV * m_PointsByHeight/2, initialPosition.z, 0 } };
			vertex.oldPosition = vertex.position;
			vertex.velocity = glm::vec4{0};
			
			vertices.push_back (vertex);
		}
	}
}

// Indicize vertices procedurally
void Cloth::InitializeIndices ()
{
	auto& indices = m_Mesh.GetIndices();

	indices.clear();

	for(int h = 0; h < m_PointsByHeight - 1; ++h){
	    for(int w = 0; w < m_PointsByWidth - 1; ++w) {
			unsigned int top_left = w % m_PointsByWidth + h * m_PointsByWidth;
			unsigned int top_right = top_left + 1;
			unsigned int bot_left = top_left + m_PointsByWidth;
			unsigned int bot_right = bot_left + 1;
			indices.push_back(top_left);
			indices.push_back(bot_right);
			indices.push_back(top_right);
			indices.push_back(top_left);
			indices.push_back(bot_left);
			indices.push_back(bot_right);
	    }
	}

}

// Set all the starting uniforms
void Cloth::Create ()
{
	MassSpring::Create();

	simulationStageComputeShader.Use();
	simulationStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);
	simulationStageComputeShader.SetUniform<GLfloat> ("drag", m_Parameters.drag);
	simulationStageComputeShader.SetUniform<glm::vec4> ("gridDims", glm::vec4 (m_PointsByWidth, m_PointsByHeight, 0, 0));
	simulationStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);
	simulationStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLengthHV);
	simulationStageComputeShader.SetUniform<GLfloat> ("restLenDiagonal", m_RestLengthDiagonal);
	simulationStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);
	simulationStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);
	simulationStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);
	simulationStageComputeShader.SetUniform<glm::vec4> ("gravityAcceleration", glm::inverse(GetTransform().GetUpdatedModelMatrix()) * m_Parameters.gravityAccel);

	constraintsStageComputeShader.Use();
	constraintsStageComputeShader.SetUniform<glm::vec4> ("gridDims", glm::vec4 (m_PointsByWidth, m_PointsByHeight, 0, 0));
	constraintsStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLengthHV);
	constraintsStageComputeShader.SetUniform<GLfloat> ("restLenDiagonal", m_RestLengthDiagonal);
	constraintsStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.correctionDumping", m_Parameters.constraintDistanceDumping);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.constraintDistanceMult", m_Parameters.constraintDistanceMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.selfCollisionDistanceMult", m_Parameters.selfCollisionDistanceMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDistMult", m_Parameters.sphereRepulsionDistMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDamping", m_Parameters.sphereRepulsionDamping);

	m_Transform.SetPosition (preset->startingPos);
	m_Transform.SetRotation (preset->startingRot);
}

void Cloth::Update ()
{
	MassSpring::Update();

	// Fixed delta time check
	if (Physics::deltaTime >= 1.0)
	{
		// Execute N substeps to reduce the simulation deltatime 
		for (int i = 0; i < m_Parameters.subSteps; i++)
		{
			simulationStageComputeShader.Use();
			BindComputeBuffers (0, 1);

			simulationStageComputeShader.Compute();
			simulationStageComputeShader.Wait();

			// SwapComputeBuffers();

			constraintsStageComputeShader.Use();
			BindComputeBuffers (1, 0);

			constraintsStageComputeShader.Compute();
			constraintsStageComputeShader.Wait();
		}

		Physics::deltaTime--;
	}

	// Update uniforms
	simulationStageComputeShader.Use();
	simulationStageComputeShader.SetUniform<glm::vec4> ("gravityAcceleration", glm::inverse(GetTransform().GetUpdatedModelMatrix()) * m_Parameters.gravityAccel);
	simulationStageComputeShader.SetUniform<GLfloat> ("drag", m_Parameters.drag);
	simulationStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);
	simulationStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);
	simulationStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);
	simulationStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);

	constraintsStageComputeShader.Use();
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.correctionDumping", m_Parameters.constraintDistanceDumping);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.constraintDistanceMult", m_Parameters.constraintDistanceMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.selfCollisionDistanceMult", m_Parameters.selfCollisionDistanceMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDistMult", m_Parameters.sphereRepulsionDistMult);
	constraintsStageComputeShader.SetUniform<GLfloat>("constraintParams.sphereRepulsionDamping", m_Parameters.sphereRepulsionDamping);
}

void Cloth::Reset ()
{
	MassSpring::Reset();
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_ComputeTempVertexBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);
}

void Cloth::SetComputeBuffers ()
{
	// Compute stage 1: compute new positions without constraints
	simulationStageComputeShader.CreateProgram ({ "clothSimulations.comp", ShaderType::COMPUTE });
	simulationStageComputeShader.SetWorkGroupSize ({ 16, 16, 1 });
	simulationStageComputeShader.SetWorkGroupNum ({ GetClothSize(), 1 });

	// Compute stage 2: apply constraints
	constraintsStageComputeShader.CreateProgram ({ "clothConstraints.comp", ShaderType::COMPUTE });
	constraintsStageComputeShader.SetWorkGroupSize ({ 16, 16, 1 });
	constraintsStageComputeShader.SetWorkGroupNum ({ GetClothSize(), 1 });

	glGenBuffers (1, &m_ComputeTempVertexBuffer);

	simulationStageComputeShader.Use();

	//In
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 0, m_Mesh.m_vbo.GetID());
	glBufferData (GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);

	// Out
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1, m_ComputeTempVertexBuffer);
	glBufferData (GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);
}

void Cloth::BindComputeBuffers (int vboBind, int tempBind)
{
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, vboBind, m_Mesh.m_vbo.GetID());
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, tempBind, m_ComputeTempVertexBuffer);
}


// Various pin edges presets

void Cloth::PinAllEdges ()
{
	auto& vertices = m_Mesh.GetVertices();

	for (int i = 0; i < vertices.size(); i += m_PointsByWidth * 2)
	{
		vertices[i + m_PointsByWidth - 1].pinned = { 1, 0, 0, 0 };
		vertices[i].pinned = { 1, 0, 0, 0 };
	}

	for (int i = 0; i < m_PointsByWidth; i += 2)
	{
		int base = LinearIndex (m_PointsByHeight - 1, 0, m_PointsByWidth);
		vertices[i + base].pinned = { 1, 0, 0, 0 };
		vertices[i].pinned = { 1, 0, 0, 0 };
	}
}

void Cloth::PinTopEdge ()
{
	auto& vertices = m_Mesh.GetVertices();

	for (int i = 0; i < m_PointsByWidth; i += 8)
	{
		int base = LinearIndex (m_PointsByHeight - 1, 0, m_PointsByWidth);
		vertices[i + base].pinned = { 1, 0, 0, 0 };
	}
}

void Cloth::PinLeftEdge ()
{
	auto& vertices = m_Mesh.GetVertices();

	for (int i = 0; i < m_PointsByHeight; i += 1)
	{
		vertices[LinearIndex(i, 0, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	}
}

void Cloth::PinLeftBorderVertices ()
{
	auto& vertices = m_Mesh.GetVertices();
	
	vertices[LinearIndex(0, 0, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex(m_PointsByHeight - 1, 0, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
}

void Cloth::PinCircleCenter ()
{
	auto& vertices = m_Mesh.GetVertices();

	vertices[LinearIndex (m_PointsByHeight / 2, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2 + m_PointsByHeight * 0.2, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2 - m_PointsByHeight * 0.2, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2, m_PointsByWidth / 2 + 10, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2, m_PointsByWidth / 2 - 10, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
}

void Cloth::PinCenter()
{
	auto& vertices = m_Mesh.GetVertices();
	vertices[LinearIndex (m_PointsByHeight / 2, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
}

void Cloth::PinTopPoints ()
{
	auto& vertices = m_Mesh.GetVertices();

	auto& topLeft = vertices[LinearIndex (m_PointsByHeight * 0.9f, 0, m_PointsByWidth)];
	auto& topRight = vertices[LinearIndex (m_PointsByHeight * 0.9f, m_PointsByWidth - 1, m_PointsByWidth)];
	auto& topCenter = vertices[LinearIndex (m_PointsByHeight * 0.9f, m_PointsByWidth / 2, m_PointsByWidth)];

	topLeft.pinned = { 1, 0, 0, 0 };
	topRight.pinned = { 1, 0, 0, 0 };
	topCenter.pinned = { 1, 0, 0, 0 };
}

void Cloth::PinFourBorderPoints ()
{
	auto& vertices = m_Mesh.GetVertices();

	auto& topLeft = vertices[LinearIndex (m_PointsByHeight - 1, 0, m_PointsByWidth)];
	auto& topRight = vertices[LinearIndex (m_PointsByHeight - 1, m_PointsByWidth - 1, m_PointsByWidth)];

	auto& bottomLeft = vertices[LinearIndex (0, 0, m_PointsByWidth)];
	auto& bottomRight = vertices[LinearIndex (0, m_PointsByWidth - 1, m_PointsByWidth)];

	topLeft.pinned = { 1, 0, 0, 0 };
	topRight.pinned = { 1, 0, 0, 0 };
	bottomLeft.pinned = { 1, 0, 0, 0 };
	bottomRight.pinned = { 1, 0, 0, 0 };
}

