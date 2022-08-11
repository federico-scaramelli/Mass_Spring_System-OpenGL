#pragma once

#include <random>

#include "Material.h"
#include "Mesh.h"
#include "Transform.h"

static std::random_device rd;
static std::mt19937 cpuGenerator(rd());
static std::uniform_real_distribution<float> unif{0, 1.0};

class GameObject {
protected:
	Transform m_Transform;
	Mesh m_Mesh;
	Material m_Material;

public:
	GameObject()
	{
		auto& vertices = m_Mesh.GetVertices();
		vertices.clear();

		std::vector<Vertex> cubeVertices = {
			{{-0.5f, -0.5f, 0.5f}}, //0
			{{0.5f, -0.5f, 0.5f}}, //1 
			{{0.5f, 0.5f, 0.5f}}, //2 
			{{-0.5f, 0.5f, 0.5f}}, //3 
			{{-0.5f, -0.5f, -0.5f}}, //4 
			{{0.5f, -0.5f, -0.5f}}, //5 
			{{0.5f, 0.5f, -0.5f}}, //6 
			{{-0.5f, 0.5f, -0.5f}} //7 
		};

		vertices.assign(cubeVertices.begin(), cubeVertices.end());

		auto& indices = m_Mesh.GetIndices();
		indices = {
			//front
			0, 1, 2,
			2, 3, 0,

			//right
			1, 5, 6,
			6, 2, 1,

			//left
			0, 3, 7,
			7, 4, 0,

			//up
			3, 2, 6,
			6, 7, 3,

			//down
			0, 4, 5,
			5, 1, 0,

			//back 
			5, 4, 7,
			7, 6, 5
		};
	}

	Transform& GetTransform()
	{
		return m_Transform;
	}

	Mesh& GetMesh()
	{
		return m_Mesh;
	}

	Material& GetMaterial()
	{
		return m_Material;
	}

	glm::vec3 GetRandomColor()
	{
		return {
			unif(cpuGenerator),
			unif(cpuGenerator),
			unif(cpuGenerator)
		};
	}

	void SetColor(glm::vec3 color)
	{
		this->m_Material.m_DiffuseColor = color;
		this->m_Material.m_AmbientColor = color;

		for (Vertex& v : m_Mesh.GetVertices())
		{
			v.color = color;
		}

		m_Mesh.UpdateBuffers();
	}
};
