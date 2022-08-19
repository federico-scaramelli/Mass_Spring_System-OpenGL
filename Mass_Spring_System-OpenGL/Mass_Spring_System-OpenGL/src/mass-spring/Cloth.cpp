#include "Cloth.h"

#include <iostream>
#include <random>

#include "../engine/Vertex.h"

Cloth::Cloth (uint16_t pointsByWidth, uint16_t pointsByHeight, float restLenghtHV) :
	GameObject ("Cloth"), m_PointsByWidth (pointsByWidth), m_PointsByHeight (pointsByHeight),
	m_RestLengthHV (restLenghtHV)
{
	float vertexCount = m_PointsByWidth * m_PointsByHeight;

	InitializeVertices();
	InitializeIndices();

	restLengthDiagonal = static_cast<GLfloat> (sqrt (pow (m_RestLengthHV, 2) * 2));
}

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
				0,
				row * m_RestLengthHV

			};

			Vertex vertex{ { initialPosition.x, initialPosition.y, initialPosition.z, 0 } };
			vertex.oldPosition = vertex.position;


			vertices.push_back (vertex);
		}
	}
}

void Cloth::InitializeIndices ()
{
	auto& indices = m_Mesh.GetIndices();

	indices.clear();

	//Riga
	for (auto x = 0; x < m_PointsByHeight - 1; x++)
	{
		//Colonna
		for (auto y = 1; y < m_PointsByWidth; y++)
		{
			int v = LinearIndex (x, y, m_PointsByWidth);
			int vLeft = v - 1;
			int vUp = v + m_PointsByWidth;
			int vUpLeft = vUp - 1;

			indices.push_back (vLeft);
			indices.push_back (v);
			indices.push_back (vUp);

			indices.push_back (vUp);
			indices.push_back (vUpLeft);
			indices.push_back (vLeft);

			// indices.push_back (vLeft);
			// indices.push_back (vUpLeft);
			// indices.push_back (vUp);
			//
			// indices.push_back (vUp);
			// indices.push_back (v);
			// indices.push_back (vLeft);
		}
	}
}

void Cloth::Create ()
{
	firstStageComputeShader.Use();

	firstStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);

	firstStageComputeShader.SetUniform<GLfloat> ("damping", m_Parameters.damping);

	firstStageComputeShader.SetUniform<glm::vec4> ("gravityAcceleration", m_Parameters.gravityAccel);

	firstStageComputeShader.SetUniform<glm::vec4> ("gridDims", glm::vec4 (m_PointsByWidth, m_PointsByHeight, 0, 0));

	firstStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);

	firstStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLengthHV);

	firstStageComputeShader.SetUniform<GLfloat> ("restLenDiagonal", restLengthDiagonal);

	firstStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);

	firstStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);

	firstStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);

	secondStageComputeShader.Use();

	secondStageComputeShader.SetUniform<glm::vec4> ("gridDims", glm::vec4 (m_PointsByWidth, m_PointsByHeight, 0, 0));

	secondStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLengthHV);

	secondStageComputeShader.SetUniform<GLfloat> ("restLenDiagonal", restLengthDiagonal);

	secondStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);

	if (collidingSphere != nullptr) { UpdateCollidingSphereUniforms(); }
}

void Cloth::UpdateCollidingSphereUniforms ()
{
	auto spherePos = glm::inverse (GetTransform().GetModelMatrix())
		* glm::vec4 (collidingSphere->GetTransform().GetPosition(), 1);

	secondStageComputeShader.Use();
	secondStageComputeShader.SetUniform<glm::vec4> ("sphereCenter", spherePos);
	secondStageComputeShader.SetUniform<GLfloat> ("sphereRadius", collidingSphere->size);
}

void Cloth::Update ()
{
	if (collidingSphere != nullptr) { UpdateCollidingSphereUniforms(); }

	for (int i = 0; i < m_Parameters.subSteps; i++)
	{
		firstStageComputeShader.Use();
		BindComputeBuffers (0, 1);

		firstStageComputeShader.Compute();
		firstStageComputeShader.Wait();

		// SwapComputeBuffers();

		secondStageComputeShader.Use();
		BindComputeBuffers (1, 0);

		secondStageComputeShader.Compute();
		secondStageComputeShader.Wait();
	}

	/*glMemoryBarrier (GL_BUFFER_UPDATE_BARRIER_BIT);
	std::vector<Vertex> copy;
	copy.resize (GetMesh().GetVertices().size());
	glad_glGetNamedBufferSubData (GetMesh().m_vbo.GetID(), 0, GetMesh().m_vbo.GetSize(), copy.data());
	std::cout << "( " << copy[0].position.x << " " << copy[0].position.y << " " << copy[0].position.z << " ) - " << copy
		[0].pinned.x << "\n";*/
	/*glad_glGetNamedBufferSubData(m_ComputeTempVertexBuffer,0, GetMesh().m_vbo.GetSize(), copy.data());
	std::cout << "( " << copy[0].position.x << " " << copy[0].position.y << " " <<  copy[0].position.z << " ) - " << copy[0].pinned.x << "\n";
	std::cout << " ----- \n";*/
}

void Cloth::SetComputeBuffers ()
{
	glGenBuffers (1, &m_ComputeTempVertexBuffer);

	firstStageComputeShader.Use();
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

void Cloth::SwapComputeBuffers ()
{
	static int swapped = 0;

	swapped = 1 - swapped;

	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, swapped, m_Mesh.m_vbo.GetID());
	glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 1 - swapped, m_ComputeTempVertexBuffer);
}

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

	for (int i = 0; i < m_PointsByWidth; i += 2)
	{
		int base = LinearIndex (m_PointsByHeight - 1, 0, m_PointsByWidth);
		vertices[i + base].pinned = { 1, 0, 0, 0 };
	}
}

void Cloth::PinCircleCenter ()
{
	auto& vertices = m_Mesh.GetVertices();

	vertices[LinearIndex (m_PointsByHeight / 2, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2 + 10, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
	vertices[LinearIndex (m_PointsByHeight / 2 - 10, m_PointsByWidth / 2, m_PointsByWidth)].pinned = { 1, 0, 0, 0 };
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
