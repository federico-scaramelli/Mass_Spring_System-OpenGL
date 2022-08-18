#include "Cloth.h"

#include <iostream>
#include <random>

#include "../engine/Vertex.h"

Cloth::Cloth(GLfloat clothWidth, GLfloat clothHeight) :
	m_Width(clothWidth), m_Height(clothHeight),
	GameObject("Cloth")
{
	density = 2.56f;
	float vertexCount = density * (m_Width * m_Height);
	float desiredMass = 500;
	particleMass = desiredMass / vertexCount;
	clothMass = vertexCount * particleMass;

	m_PointsByWidth = static_cast<int>(sqrt(vertexCount));
	m_PointsByHeight = m_PointsByWidth;
	

	InitializeVertices();
	InitializeIndices();

	//Pin the 4 outern vertices
	auto& vertices = m_Mesh.GetVertices();

	auto& topLeft = vertices[LinearIndex(m_PointsByHeight -1, 0, m_PointsByWidth)];
	auto& topRight = vertices[LinearIndex(m_PointsByHeight -1, m_PointsByWidth -1, m_PointsByWidth)];
	auto& topCenter = vertices[LinearIndex(m_PointsByHeight -1, m_PointsByWidth/2, m_PointsByWidth)];


	for (int i = 0 ; i < m_PointsByWidth; i+=6)
	{
		int base = LinearIndex(m_PointsByHeight - 1, 0, m_PointsByWidth);
		vertices[i + base].pinned={1,0,0,0};
		vertices[i + base + 1].pinned={1,0,0,0};
	}


	restLengthVertical = m_Height / m_PointsByHeight;
	restLengthHorizontal = m_Width / m_PointsByWidth;
	restLengthDiagonal = static_cast<GLfloat>(sqrt(pow(restLengthVertical, 2) + pow(restLengthHorizontal, 2)));
	stiffness = 5000.f;
	kSheering = 1.5;
	kBending = kSheering * 0.2f;


	// topLeft.pinned = {1, 0, 0, 0};
	// topRight.pinned = {1, 0, 0, 0};
	// topCenter.pinned = {1, 0, 0, 0};
}

void Cloth::InitializeVertices()
{
	auto& vertices = m_Mesh.GetVertices();

	vertices.clear();

	float spacingWidth = m_Width / m_PointsByWidth;
	float spacingHeight = m_Height / m_PointsByHeight;

	for (auto row = 0; row < m_PointsByHeight; row++)
	{
		for (auto column = 0; column < m_PointsByWidth; column++)
		{
			glm::vec3 initialPosition{
				column * spacingWidth,
				0,
				row * spacingHeight
			};

			Vertex vertex{{initialPosition.x, initialPosition.y, initialPosition.z, 0}};
			vertex.oldPosition = vertex.position;


			vertices.push_back(vertex);
		}
	}
}

void Cloth::InitializeIndices()
{
	auto& indices = m_Mesh.GetIndices();

	indices.clear();

	//Riga
	for (auto x = 0; x < m_PointsByHeight - 1; x++)
	{
		//Colonna
		for (auto y = 1; y < m_PointsByWidth; y++)
		{
			int v = LinearIndex(x, y, m_PointsByWidth);
			int vLeft = v - 1;
			int vUp = v + m_PointsByWidth;
			int vUpLeft = vUp - 1;

			indices.push_back(vLeft);
			indices.push_back(v);
			indices.push_back(vUp);

			indices.push_back(vUp);
			indices.push_back(vUpLeft);
			indices.push_back(vLeft);

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

void Cloth::Create()
{
	firstStageComputeShader.Use();

	firstStageComputeShader.SetUniform<GLfloat>("deltaTime", m_Parameters.deltaTime);

	firstStageComputeShader.SetUniform<GLfloat>("damping", m_Parameters.damping);

	firstStageComputeShader.SetUniform<glm::vec4>("gravityAcceleration", m_Parameters.gravityAccel);

	firstStageComputeShader.SetUniform<glm::vec4>("gridDims", glm::vec4(m_PointsByWidth, m_PointsByHeight, 0, 0));

	firstStageComputeShader.SetUniform<GLfloat>("elasticStiffness", stiffness);

	firstStageComputeShader.SetUniform<GLfloat>("restLenHorizontal", restLengthHorizontal);

	firstStageComputeShader.SetUniform<GLfloat>("restLenVertical", restLengthVertical);

	firstStageComputeShader.SetUniform<GLfloat>("restLenDiagonal", restLengthDiagonal);

	firstStageComputeShader.SetUniform<GLfloat>("particleMass", clothMass);

	firstStageComputeShader.SetUniform<GLfloat>("constShearMult", kSheering);

	firstStageComputeShader.SetUniform<GLfloat>("constBendMult", kBending);

	secondStageComputeShader.Use();

	secondStageComputeShader.SetUniform<glm::vec4>("gridDims", glm::vec4(m_PointsByWidth, m_PointsByHeight, 0, 0));

	secondStageComputeShader.SetUniform<GLfloat>("restLenHorizontal", restLengthHorizontal);

	secondStageComputeShader.SetUniform<GLfloat>("restLenVertical", restLengthVertical);

	secondStageComputeShader.SetUniform<GLfloat>("restLenDiagonal", restLengthDiagonal);

	secondStageComputeShader.SetUniform<GLfloat>("deltaTime", m_Parameters.deltaTime);
}

void Cloth::Update()
{
	int maxIterations = 100;

	for (int i = 0; i < maxIterations; i++)
	{
		firstStageComputeShader.Use();
		BindComputeBuffers(0, 1);

		firstStageComputeShader.Compute();
		firstStageComputeShader.Wait();

		// SwapComputeBuffers();

		secondStageComputeShader.Use();
		BindComputeBuffers(1, 0);

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

void Cloth::SetComputeBuffers()
{
	glGenBuffers(1, &m_ComputeTempVertexBuffer);

	firstStageComputeShader.Use();
	//In
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_Mesh.m_vbo.GetID());
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);

	// Out
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ComputeTempVertexBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_Mesh.m_vbo.GetSize(), m_Mesh.GetVertices().data(), GL_DYNAMIC_DRAW);

	// secondStageComputeShader.Use();
	// //In
	// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 2, m_ComputeTempVertexBuffer);
	// //Out
	// glBindBufferBase (GL_SHADER_STORAGE_BUFFER, 3, m_Mesh.m_vbo.GetID());
}

void Cloth::BindComputeBuffers(int vboBind, int tempBind)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, vboBind, m_Mesh.m_vbo.GetID());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, tempBind, m_ComputeTempVertexBuffer);
}

void Cloth::SwapComputeBuffers()
{
	static int swapped = 0;

	swapped = 1 - swapped;

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, swapped, m_Mesh.m_vbo.GetID());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1 - swapped, m_ComputeTempVertexBuffer);
}
