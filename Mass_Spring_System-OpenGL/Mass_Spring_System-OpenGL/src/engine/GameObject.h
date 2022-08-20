#pragma once

#include <random>

#include "GameObjectUI.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Transform.h"

static std::random_device rd;
static std::mt19937 cpuGenerator(rd());
static std::uniform_real_distribution<float> unif{ 0, 1.0 };

class GameObject {
protected:
	Transform m_Transform;
	Mesh m_Mesh{};
	GameObjectUI* m_GameObjectUI;

public:
	virtual ~GameObject () { delete m_GameObjectUI; }
	const char* name;
	bool m_IsActive = true;
	
	GameObject(const char* name) : m_Transform (name), name (name) {}

	Transform& GetTransform() {
		return m_Transform;
	}

	Mesh& GetMesh() {
		return m_Mesh;
	}

	virtual void GenerateUI()
	{
		m_Transform.GenerateUI(m_GameObjectUI->m_TransformUI);
		GetMaterial().GenerateUI (m_GameObjectUI->m_MaterialUI);
	}

	virtual void UpdateWithUI() {
		m_Transform.UpdateWithUI();

		m_Mesh.GetMaterial().UpdateWithUI();
	}

	glm::vec3 GetRandomColor() {
		return {
			unif(cpuGenerator),
			unif(cpuGenerator),
			unif(cpuGenerator)
		};
	}

	void SetColor(glm::vec3 color) {
		/*this->m_Mesh.GetMaterial().m_DiffuseColor = color;
		this->m_Mesh.GetMaterial().m_AmbientColor = color;

		for (Vertex& v : m_Mesh.GetVertices())
		{
			v.color = color;
		}

		m_Mesh.UpdateBuffers();*/
	}

	ShaderProgram& GetShader() {
		return m_Mesh.GetMaterial().GetShader();
	}

	GameObjectUI& GetUI() { return *m_GameObjectUI; }

	virtual void Create() = 0;
	virtual void Update() = 0;

	Material& GetMaterial() {
		return m_Mesh.GetMaterial();
	}
};
