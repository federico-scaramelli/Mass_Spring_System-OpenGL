#pragma once

#include <random>

#include "GameObjectUI.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Transform.h"
#include "VertexBufferLayout.h"

class GameObject
{
protected:
	Transform m_Transform;
	Mesh m_Mesh{};
	GameObjectUI* m_GameObjectUI = nullptr;
	VertexBufferLayout vertexBufferLayout{};

public:
	virtual ~GameObject () { delete m_GameObjectUI; }
	const char* name;
	bool m_IsActive = true;

	GameObject (const char* name) : m_Transform (name), name (name) {}

	Transform& GetTransform () { return m_Transform; }

	Mesh& GetMesh () { return m_Mesh; }

	virtual void GenerateUI ();

	virtual void UpdateWithUI ();

	ShaderProgram& GetShader () { return m_Mesh.GetMaterial().GetShader(); }

	GameObjectUI& GetUI () { return *m_GameObjectUI; }

	void SetupGraphicsShader ();

	virtual void Create () = 0;
	virtual void Update () = 0;

	Material& GetMaterial () { return m_Mesh.GetMaterial(); }
};
