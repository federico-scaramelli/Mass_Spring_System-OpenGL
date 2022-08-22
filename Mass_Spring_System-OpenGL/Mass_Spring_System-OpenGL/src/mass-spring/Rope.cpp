#include "Rope.h"

struct Node {
	glm::vec4 position;
	glm::vec4 velocity;
	glm::vec4 oldPosition;
	glm::vec4 pinned;
};

Rope::Rope(GLint pointsByLength, uint16_t restLenght, GLfloat radius) :
	MassSpring ("Rope", MassSpringParameters (0.016f, 16, 0.98f, 
													{ 0.f, -200, 0.f, 0.f }, 
													1.0f, 10.0f, 1.0f, 1.0f)),
	m_RestLength (restLenght), m_PointsByLength (pointsByLength),
	m_Radius (radius)
{
	InitializeNodes();
	Rope::InitializeVertices();
	Rope::InitializeIndices();
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

		m_Nodes.push_back({{initialPosition, 1.0f}});
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
			glm::vec3 positionSide = nextNodeRightDir * weigth.x * m_Radius;
			glm::vec3 positionFront = nextNodeUpDir * weigth.y * m_Radius;

			glm::vec3 position = positionSide + positionFront;
			position += glm::vec3(m_Nodes[i].position);

			Vertex vertex{{position.x, position.y, position.z, 0}};

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

void Rope::Create()
{
	simulationStageComputeShader.Use();

	simulationStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);

	simulationStageComputeShader.SetUniform<GLfloat> ("damping", m_Parameters.damping);
	
	simulationStageComputeShader.SetUniform<GLfloat> ("ropeDim", m_PointsByLength);

	simulationStageComputeShader.SetUniform<GLfloat> ("elasticStiffness", m_Parameters.stiffness);

	simulationStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLength);

	simulationStageComputeShader.SetUniform<GLfloat> ("particleMass", m_Parameters.particleMass);

	simulationStageComputeShader.SetUniform<GLfloat> ("constShearMult", m_Parameters.kSheering);

	simulationStageComputeShader.SetUniform<GLfloat> ("constBendMult", m_Parameters.kBending);

	constraintsStageComputeShader.Use();
	
	constraintsStageComputeShader.SetUniform<GLfloat> ("verticesDim", m_Mesh.GetVertices().size());
	//
	// constraintsStageComputeShader.SetUniform<GLfloat> ("restLenHV", m_RestLength);
	//
	// constraintsStageComputeShader.SetUniform<GLfloat> ("deltaTime", m_Parameters.subStepDt);
}

void Rope::Update()
{


}
