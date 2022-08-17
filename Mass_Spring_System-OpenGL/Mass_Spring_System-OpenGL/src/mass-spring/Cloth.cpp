#include "Cloth.h"

#include <iostream>
#include <random>

#include "../engine/Vertex.h"

Cloth::Cloth(GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight) :
	m_Width(clothWidth), m_Height(clothHeight), m_PointsByWidth(pointsWidth), m_PointsByHeight(pointsHeight),
	GameObject("Cloth")
{
	InitializeVertices();
	InitializeIndices();

	//Pin the 4 outern vertices
	auto& vertices = m_Mesh.GetVertices();

	auto& topLeft = vertices[LinearIndex(m_PointsByHeight - 1, 0, m_PointsByWidth)];
	auto& topRight = vertices[LinearIndex(m_PointsByHeight - 1, m_PointsByWidth - 1, m_PointsByWidth)];
	auto& bottomLeft = vertices[LinearIndex(0, 0, m_PointsByWidth - 1)];
	auto& bottomRight = vertices[LinearIndex(0, m_PointsByWidth - 1, m_PointsByWidth)];

	density=1.f;

	restLengthVertical = m_Height / m_PointsByHeight;
	restLengthHorizontal = m_Width / m_PointsByWidth;
	restLengthDiagonal = static_cast<GLfloat>(sqrt(pow(restLengthVertical, 2) + pow(restLengthHorizontal, 2)));
	particleMass=1.0;
	stiffness=5000.f;
	kSheering=1.5;
	kBending=kSheering*0.2f;
	clothMass=static_cast<float>(vertices.size()) * particleMass * density;
	

	topLeft.pinned = {1, 0, 0, 0};
	topRight.pinned = {1, 0, 0, 0};
	bottomRight.pinned = {1, 0, 0, 0};
	bottomLeft.pinned = {1, 0, 0, 0};

	// topLeft.position.x -= m_Width * 2.f;
	// topRight.position.x += m_Width * 2.f;
	//
	// bottomLeft.position.x -= m_Width * 2.f;
	// bottomRight.position.x += m_Width * 2.f;
	// bottomLeft.position.y -= m_Height * 5.f;
	// bottomRight.position.y -= m_Height * 5.f;
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
				row * spacingHeight,
				row * 0.3f
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

	firstStageComputeShader.SetUniform<GLfloat>("elasticStiffness", stiffness);

	firstStageComputeShader.SetUniform<GLfloat>("restLenHorizontal", restLengthHorizontal);

	firstStageComputeShader.SetUniform<GLfloat>("restLenVertical", restLengthVertical);

	firstStageComputeShader.SetUniform<GLfloat>("restLenDiagonal", restLengthDiagonal);

	firstStageComputeShader.SetUniform<GLfloat>("particleMass", clothMass);

	firstStageComputeShader.SetUniform<GLfloat>("constShearMult", kSheering);

	firstStageComputeShader.SetUniform<GLfloat>("constBendMult", kBending);

	// secondStageComputeShader.Use();

	// secondStageComputeShader.SetUniform<GLfloat>("restLenHorizontal", m_Parameters.restLengthHorizontal);
	//
	// secondStageComputeShader.SetUniform<GLfloat>("restLenVertical", m_Parameters.restLengthVertical);
	//
	// secondStageComputeShader.SetUniform<GLfloat>("restLenDiagonal", m_Parameters.restLengthDiagonal);
}

void Cloth::Update()
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
