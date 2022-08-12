#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "../engine/GameObject.h"

#define LinearIndex(i,j,rowSize) ((i)*(rowSize) + (j))

struct Vertex;

class Cloth : public GameObject {
private:
	GLfloat m_Width;
	GLfloat m_Height;
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;

public:
	Cloth(GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight);
	
	void InitializeVertices();

	void InitializeIndices();
};

