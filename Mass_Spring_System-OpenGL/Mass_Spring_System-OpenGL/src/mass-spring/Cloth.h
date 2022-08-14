#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "../engine/GameObject.h"

#define LinearIndex(i,j,rowSize) ((i)*(rowSize) + (j))

struct Vertex;

class Cloth : public GameObject {
public:
	GLfloat m_Width;
	GLfloat m_Height;
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;


	PhysicsParameters m_Parameters;

	Cloth(GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight);
	
	void InitializeVertices();

	void InitializeIndices();

	glm::vec2 GetClothSize()
	{
		return {m_PointsByWidth, m_PointsByHeight};
	}
};

