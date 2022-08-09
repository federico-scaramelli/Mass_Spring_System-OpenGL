#pragma once
#include <map>
#include <vector>

#include "glad/glad.h"
#include "glm/vec3.hpp"

struct Vertex;

class Cloth {
private:
	float width;
	float height;
	int pointsByWidth;
	int pointsByHeight;

	std::vector<Vertex*> vertices;
	std::vector<GLuint> indices;
	//for later
	// std::map<std::pair<int, int>, bool> pinnedVertices;

	glm::vec3 color{0.15f, 0.15f, 0.7f};

public:
	Cloth(float clothWidth, float clothHeight, int pointsWidth, int pointsHeight);

	std::vector<Vertex*>& GetVertices() { return vertices; }

	float GetSize() const { return vertices.size(); }

	void InitializeVertices();

	void InitializeIndices();
};

