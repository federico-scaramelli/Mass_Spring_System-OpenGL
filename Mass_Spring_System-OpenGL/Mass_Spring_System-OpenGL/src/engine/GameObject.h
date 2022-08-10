#pragma once

#include <random>

#include "Mesh.h"
#include "Transform.h"

static std::random_device rd;
static std::mt19937 cpuGenerator (rd());
static std::uniform_real_distribution<float> unif{ 0, 1.0 };

class GameObject {
protected:
	Transform m_Transform;
	Mesh m_Mesh;
	glm::vec3 color{0.15f, 0.15f, 0.7f};

public:
	Transform& GetTransform() { return m_Transform; }
	Mesh& GetMesh() { return m_Mesh; }

	glm::vec3 GetRandomColor(){
		return {
			unif (cpuGenerator),
			unif (cpuGenerator),
			unif (cpuGenerator)
	};
}
};

