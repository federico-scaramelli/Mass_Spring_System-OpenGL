#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "../engine/GameObject.h"
#include <array>
#include <utility>

class Rope : public GameObject {
private:
	GLfloat m_Length;
	GLint m_PointsByLength;
	GLfloat m_Radius;

	std::vector<glm::vec3> m_Nodes;

	std::array<glm::vec2, 8> vertexPositionsWeigths = {
		{
			{1.f,	  0.f},
			{0.7f,	  0.7f},
			{0.f,	  1.f},
			{-0.7f,	  0.7f},
			{-1.f,	  0.f},
			{-0.7f,	  -0.7f},
			{0.f,	  -1.f},
			{0.7f,	  -0.7f}
		}
	};

public:
	Rope(GLfloat length, GLint pointsByLength, GLfloat radius = 10.f);

	void InitializeNodes();

	void InitializeVertices();

	void InitializeIndices();

	void CreateFrontSurfaceIndices();

	void CreateMidSurfaceIndices();

	void CreateBackSurfaceIndices();
};
