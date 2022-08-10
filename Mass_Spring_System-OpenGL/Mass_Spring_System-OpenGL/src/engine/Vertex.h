#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color {1.f};
	glm::vec3 normal {1.f};
	glm::vec2 uv {0.f};
};