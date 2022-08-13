#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec4 position {0,0,0,0};

	glm::vec4 velocity {0,0,0,0};

	glm::vec4 color {1.f, 0.f, 0.f, 0.f};

	glm::vec4 normal {0.f, 0.f, 1.f, 1.f};

	glm::vec2 uv {0.f};

	GLfloat pinned = 0;

	GLfloat dummy;
};