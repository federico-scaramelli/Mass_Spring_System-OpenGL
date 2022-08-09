#include "Cloth.h"

#include "src/engine/Vertex.h"

Cloth::Cloth(float clothWidth, float clothHeight, int pointsWidth, int pointsHeight) :
	width(clothWidth), height(clothHeight), pointsByWidth(pointsWidth), pointsByHeight(pointsHeight)
{
	InitializeVertices();
	InitializeIndices();
}

void Cloth::InitializeVertices()
{
	vertices.clear();

	//Colonne
	for (auto i = 0; i < pointsByHeight; ++i) {
		//Righe
		for (auto j = 0; j < pointsByWidth; ++j) {
			glm::vec3 initialPosition = {
				j * width / glm::max(1, pointsByWidth - 1),
				i * height / glm::max(1, pointsByHeight - 1),
				0
			};

			Vertex* vertex = new Vertex();
			vertex->position=initialPosition;
			vertices.push_back(vertex);
		}
	}
}

void Cloth::InitializeIndices()
{
	indices.clear();


}
