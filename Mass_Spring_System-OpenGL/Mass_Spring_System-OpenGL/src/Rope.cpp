#include "Rope.h"

Rope::Rope(GLfloat length, GLint pointsByLength, GLfloat radius) :
	m_Length(length), m_PointsByLength(pointsByLength), m_Radius(radius)
{
	InitializeNodes();
	InitializeVertices();
	InitializeIndices();
}

void Rope::InitializeNodes()
{
	m_Nodes.clear();

	float spacing = m_Length / m_PointsByLength;

	for (auto i = 0; i < m_PointsByLength; i++)
	{
		glm::vec3 initialPosition{
			i * spacing,
			0,
			0
		};

		m_Nodes.push_back(initialPosition);
	}
}

void Rope::InitializeVertices()
{
	auto& vertices = m_Mesh.GetVertices();

	glm::vec3 nextNodeForwardDir = {1, 0, 0};
	glm::vec3 nextNodeUpDir = {0, 1, 0};
	glm::vec3 nextNodeRightDir = {0, 0, 1};

	//Create vertices for nodes except last one (last one gets last axis, not updated)
	for (auto i = 0; i < m_Nodes.size(); i++)
	{
		//Update axis for all nodes except last node, with next node position
		if (i < m_Nodes.size() - 1)
		{
			nextNodeForwardDir = glm::normalize(m_Nodes[i + 1] - m_Nodes[i]);
			nextNodeRightDir = glm::normalize(glm::cross(nextNodeForwardDir, Transform::worldUpDirection));
			nextNodeUpDir = glm::normalize(glm::cross(nextNodeRightDir, nextNodeForwardDir));
		}

		for (auto weigth : vertexPositionsWeigths)
		{
			glm::vec3 positionSide = nextNodeRightDir * weigth.x * m_Radius;
			glm::vec3 positionFront = nextNodeUpDir * weigth.y * m_Radius;

			glm::vec3 position = positionSide + positionFront;
			position += m_Nodes[i];

			Vertex vertex{position};
			vertex.color = GetRandomColor();

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
