#include "Cloth.h"

#include <random>

#include "engine/Vertex.h"

Cloth::Cloth (GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight) :
	m_Width (clothWidth), m_Height (clothHeight), m_PointsByWidth (pointsWidth), m_PointsByHeight (pointsHeight)
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

	//Riga
	for (auto x = 0; x < m_PointsByHeight; x++) {
		//Colonna
		for (auto y = 0; y < m_PointsByWidth; y++) {
			glm::vec3 initialPosition {
				y * spacingWidth,
				x * spacingHeight,
				0
			};

			Vertex vertex {initialPosition};
			//vertex.color = GetRandomColor();
			// vertex.color = color;

			vertices.push_back (vertex);
		}
	}
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

