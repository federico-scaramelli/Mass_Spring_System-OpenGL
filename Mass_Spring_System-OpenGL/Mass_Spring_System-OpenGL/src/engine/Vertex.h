#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position {0,0,0};
	GLfloat dummyPosition;

	glm::vec3 color {1.f, 0.f, 0.f};
	GLfloat dummyColor;

	glm::vec3 normal {0.f, 0.f, 1.f};
	GLfloat dummyNormal;

	glm::vec2 uv {0.f};
	glm::vec2 dummyUV;
};