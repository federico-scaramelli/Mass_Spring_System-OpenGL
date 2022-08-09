#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

struct QuadAttributesStruct {
	glm::vec3 position;
	glm::vec3 color;
};