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
	GameObject()
	{
		 auto& vertices=m_Mesh.GetVertices();
		 vertices.clear();

		 std::vector<Vertex> cubeVertices = {
              {{ -0.5f,  0.5f,  0.5f }},
              {{ -0.5f, -0.5f,  0.5f }},
              {{  0.5f, -0.5f,  0.5f }},
              {{  0.5f,  0.5f,  0.5f }},
              {{  0.5f,  0.5f, -0.5f }},
              {{  0.5f, -0.5f, -0.5f }},
              {{ -0.5f,  0.5f, -0.5f }},
              {{ -0.5f, -0.5f, -0.5f }}
          };

        vertices.assign (cubeVertices.begin(), cubeVertices.end());

		auto& indices=m_Mesh.GetIndices();
        indices = {
            0, 1, 2,
            2, 3, 0,

            4, 3, 2,
            5, 4, 2,

            6, 4, 5,
            7, 6, 5,

            6, 7, 0,
            1, 0, 7,

            3, 6, 0,
            4, 6, 3,

            1, 7, 2,
            7, 5, 2
        };
	}

	Transform& GetTransform() { return m_Transform; }
	Mesh& GetMesh() { return m_Mesh; }

	glm::vec3 GetRandomColor(){
		return {
				unif (cpuGenerator),
				unif (cpuGenerator),
				unif (cpuGenerator)
		};
	}

	void SetColor(glm::vec3 color)
	{
        for (Vertex& v : m_Mesh.GetVertices()) {
	        v.color = color;
        }
        m_Mesh.UpdateBuffers();
		this->color = color;
	}
};

