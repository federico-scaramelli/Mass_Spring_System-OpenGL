#pragma once
#include <map>
#include <vector>

#include "Vertex.h"
#include "glad/glad.h"

class Mesh {
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	//Map<indice vertice, se è pinnato>
	std::map<int, bool> pinnedVertices;

public:
	std::vector<Vertex>& GetVertices() { return vertices; }

	std::vector<GLuint>& GetIndices() { return indices; }

	GLfloat GetSize() const { return vertices.size(); }
};

