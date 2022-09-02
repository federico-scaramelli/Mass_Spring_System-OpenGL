#pragma once

#include <random>

#include "GameObjectUI.h"
#include "Mesh.h"
#include "Transform.h"
#include "VertexBufferLayout.h"

class GameObject
{
protected:
	Transform m_Transform;
	Mesh m_Mesh{};
	VertexBufferLayout vertexBufferLayout{};

	GameObjectUI* m_GameObjectUI = nullptr;

public:
	std::string name;
	bool m_IsActive = true;

	GameObject (const char* name) : m_Transform (name), name (name) {}
	virtual ~GameObject () { delete m_GameObjectUI; }

	Transform& GetTransform () { return m_Transform; }
	Mesh& GetMesh () { return m_Mesh; }
	Material& GetMaterial () { return m_Mesh.GetMaterial(); }
	ShaderProgram& GetShader () { return m_Mesh.GetMaterial().GetShader(); }
	GameObjectUI& GetUI () { return *m_GameObjectUI; }

	virtual void GenerateUI ();
	virtual void UpdateWithUI ();

	void SetupGraphicsShader (FragmentShader fragmentPreset);

	virtual void Create () = 0;
	virtual void Update () = 0;
};
