#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec4 position {0,0,0,0};

	glm::vec4 velocity {0,0,0,0};

	glm::vec4 color {1.f, 0.f, 0.f, 0.f};

	glm::vec4 normal {0.f, 0.f, 1.f, 1.f};

	glm::vec4 oldPosition {0.f, 0.f, 1.f, 1.f};

	glm::vec4 pinned {0,0,0,0};

};