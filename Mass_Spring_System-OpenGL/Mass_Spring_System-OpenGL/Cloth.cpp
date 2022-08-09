#include "Cloth.h"

#include <random>

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

	float spacingWidth=width/pointsByWidth;
	float spacingHeight=height/pointsByHeight;
	
	//Riga
	for (auto i = 0; i < pointsByHeight; i++) {
		//Colonna
		for (auto j = 0; j < pointsByWidth; j++) {
			glm::vec3 initialPosition = {
				j*spacingWidth,
				i*spacingHeight,
				0
			};

			Vertex* vertex = new Vertex();
			vertex->position=initialPosition;
			// vertex->color=GetRandomColor();

			vertices.push_back(vertex);
		}
	}
}

void Cloth::InitializeIndices()
{
	indices.clear();

	//Riga
	for (auto i = 0; i < pointsByHeight - 1; ++i) {
		//Colonna
		for (auto j = 1; j < pointsByWidth; ++j) {
			int v=LinearIndex(i,j, pointsByWidth);
			int vLeft=v-1;
			int vUp=v+pointsByWidth;
			int vUpLeft=vUp - 1;

			indices.push_back(vLeft);
			indices.push_back(v);
			indices.push_back(vUp);
			
			indices.push_back(vUp);
			indices.push_back(vUpLeft);
			indices.push_back(vLeft);

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
static std::mt19937 cpuGenerator(rd());
static std::uniform_real_distribution<float> unif{0, 1.0};

glm::vec3 Cloth::GetRandomColor()
{
	return {
		unif(cpuGenerator),
		unif(cpuGenerator),
		unif(cpuGenerator)
	};
}
