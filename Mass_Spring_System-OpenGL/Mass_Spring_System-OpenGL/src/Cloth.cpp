#include "Cloth.h"

#include <random>

#include "engine/Vertex.h"

Cloth::Cloth (float clothWidth, float clothHeight, int pointsWidth, int pointsHeight) :
	width (clothWidth), height (clothHeight), pointsByWidth (pointsWidth), pointsByHeight (pointsHeight)
{
	InitializeVertices();
	InitializeIndices();
}

void Cloth::InitializeVertices ()
{
	vertices.clear();

	float spacingWidth = width / pointsByWidth;
	float spacingHeight = height / pointsByHeight;

	//Riga
	for (auto x = 0; x < pointsByHeight; x++) {
		//Colonna
		for (auto y = 0; y < pointsByWidth; y++) {
			glm::vec3 initialPosition {
				y * spacingWidth,
				x * spacingHeight,
				0
			};

			Vertex vertex {initialPosition};
			vertex.color = GetRandomColor();

			vertices.push_back (vertex);
		}
	}
}

void Cloth::InitializeIndices ()
{
	indices.clear();

	//Riga
	for (auto x = 0; x < pointsByHeight - 1; x++) {
		//Colonna
		for (auto y = 1; y < pointsByWidth; y++) {
			int v = LinearIndex (x, y, pointsByWidth);
			int vLeft = v - 1;
			int vUp = v + pointsByWidth;
			int vUpLeft = vUp - 1;

			indices.push_back (vLeft);
			indices.push_back (v);
			indices.push_back (vUp);

			indices.push_back (vUp);
			indices.push_back (vUpLeft);
			indices.push_back (vLeft);

			// indices.push_back(vBottomLeft);
			// indices.push_back(vBottom);
			// indices.push_back(v);
			//
			// indices.push_back(v);
			// indices.push_back(vLeft);
			// indices.push_back(vBottomLeft);


		}
	}
}

static std::random_device rd;
static std::mt19937 cpuGenerator (rd());
static std::uniform_real_distribution<float> unif{ 0, 1.0 };

glm::vec3 Cloth::GetRandomColor ()
{
	return {
		unif (cpuGenerator),
		unif (cpuGenerator),
		unif (cpuGenerator)
	};
}
