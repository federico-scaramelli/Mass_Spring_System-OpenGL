#include "Cloth.h"

#include <random>

#include "../engine/Vertex.h"

Cloth::Cloth (GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight) :
	m_Width (clothWidth), m_Height (clothHeight), m_PointsByWidth (pointsWidth), m_PointsByHeight (pointsHeight),
	GameObject("Cloth")
{
	InitializeVertices();
	InitializeIndices();
}

void Cloth::InitializeVertices ()
{
	auto& vertices=m_Mesh.GetVertices();

	vertices.clear();

	float spacingWidth = m_Width / m_PointsByWidth;
	float spacingHeight = m_Height / m_PointsByHeight;
	
	for (auto row = 0; row < m_PointsByHeight; row++) {
		for (auto column = 0; column < m_PointsByWidth; column++) {
			glm::vec3 initialPosition {
				column * spacingWidth,
				row * spacingHeight,
				0
			};

			Vertex vertex {{initialPosition.x, initialPosition.y, initialPosition.z, 0}};
			// vertex.pinned={1,0,0,0};

			vertices.push_back (vertex);
		}
	}

	//Pin the 4 outern vertices
	vertices[LinearIndex(0,0,m_PointsByWidth-1)].pinned={1,0,0,0};
	vertices[LinearIndex(0,m_PointsByWidth-1,m_PointsByWidth)].pinned={1,0,0,0};
	vertices[LinearIndex(m_PointsByHeight-1,0,m_PointsByWidth)].pinned={1,0,0,0};
	vertices[LinearIndex(m_PointsByHeight-1,m_PointsByWidth-1,m_PointsByWidth)].pinned={1,0,0,0};
}

void Cloth::InitializeIndices ()
{
	auto& indices=m_Mesh.GetIndices();

	indices.clear();

	//Riga
	for (auto x = 0; x < m_PointsByHeight - 1; x++) {
		//Colonna
		for (auto y = 1; y < m_PointsByWidth; y++) {
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
		}
	}
}

