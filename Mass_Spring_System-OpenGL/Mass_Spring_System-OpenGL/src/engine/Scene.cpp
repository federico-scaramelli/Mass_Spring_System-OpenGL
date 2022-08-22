#include "Scene.h"

#include <iostream>

#include "Camera.h"
#include "LightSource.h"
#include "../mass-spring/Wind.h"
#include "Renderer.h"
#include "CollidingSphere.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/glm.hpp"
#include "../mass-spring/Cloth.h"
#include "../mass-spring/PhysicsSolver.h"

Scene* Scene::GetInstance ()
{
	if (instance == nullptr)
	{
		instance = new Scene();
	}
	return instance;
}

void Scene::AddCamera (Camera* camera)
{
	m_Camera = camera;

	camera->GenerateUI();
	camera->GetTransform().SetPosition ({ 0, -100, 700 });
}

void Scene::AddGameObject (Primitive* primitive)
{
	primitive->SetupGraphicsShader(BlinnPhong);
	m_Primitives.push_back (primitive);
	
	primitive->Create();
	primitive->GenerateUI();
}

void Scene::AddGameObject (CollidingSphere* collidingSphere)
{
	collidingSphere->SetupGraphicsShader(BlinnPhong);
	m_Primitives.push_back (collidingSphere);
	m_PhysicsSolver.AddCollider (collidingSphere);
	
	collidingSphere->Create();
	collidingSphere->GenerateUI();
}

void Scene::AddGameObject (MassSpring* massSpring)
{
	massSpring->SetupGraphicsShader(BlinnPhong);
	MassSpringUI::sceneMassSprings[m_MassSprings.size()] = massSpring->name;
	m_MassSprings.push_back (massSpring);

	massSpring->SetComputeBuffers();
	massSpring->Create();
	massSpring->GenerateUI();
}

void Scene::AddGameObject (Wind* wind)
{
	wind->SetupGraphicsShader(Solid);
	m_Wind = wind;
	m_PhysicsSolver.SetActiveWind (wind);

	wind->GenerateUI();
}

void Scene::AddGameObject (LightSource* light)
{
	light->SetupGraphicsShader(Solid);
	m_LightSource = light;
	light->GenerateUI();
}

void Scene::UpdateCamera () { m_Camera->UpdateWithUI(); }

void Scene::Update ()
{
	if (m_Camera == nullptr) { throw std::runtime_error ("Camera not present!"); }

	UpdateCamera();
	UpdateGameObjects();

	m_PhysicsSolver.Update();
}

void Scene::UpdateGameObjects ()
{
	// Update primitives
	for (auto primitive : m_Primitives)
	{
		if (!primitive->m_IsActive) continue;

		// Custom update behavior of game object
		primitive->Update();

		UpdateTransform (primitive);
		DrawGameObject (primitive);
	}
	
	// Update mass springs
	if (!m_MassSprings.empty())
	{
		auto massSpring = m_MassSprings[MassSpringUI::selectedMassSpring];
		m_PhysicsSolver.SetActiveMassSpring (massSpring);
		massSpring->Update();
		UpdateTransform (massSpring);
		DrawGameObject (massSpring);
	}

	// Update Wind
	if (m_Wind != nullptr)
	{
		m_Wind->GetMesh().GetMaterial().GetShader().Use();
		UpdateTransform (m_Wind);
		m_Wind->Update();
		DrawGameObject (m_Wind);
	}

	// Update Light
	if (m_LightSource != nullptr)
	{
		m_LightSource->GetMesh().GetMaterial().GetShader().Use();
		UpdateTransform (m_LightSource);
		DrawGameObject (m_LightSource);
	}
}

void Scene::UpdateTransform (GameObject* gameObject)
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	gameObject->GetMaterial().Update();
	gameObject->UpdateWithUI();

	modelViewMatrix = viewMatrix * gameObject->GetTransform().GetUpdatedModelMatrix();
	gameObject->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);
	if (gameObject->GetMaterial().fragShader == BlinnPhong || gameObject->GetMaterial().fragShader == Normal)
	{
		gameObject->GetShader().SetUniform<glm::mat3> ("normalMatrix",
													  glm::inverseTranspose (glm::mat3 (modelViewMatrix)));
	}
}

void Scene::DrawGameObject (GameObject* gameObject)
{
	Renderer::GetInstance()->Draw (gameObject->GetMesh().m_vao,
	                  gameObject->GetMesh().m_indexBuffer,
	                  gameObject->GetShader());
}

Scene* Scene::instance = nullptr;
