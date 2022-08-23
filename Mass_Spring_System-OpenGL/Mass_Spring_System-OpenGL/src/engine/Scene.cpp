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
#include "../mass-spring/MassSpringUI.h"

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

void Scene::AddGameObject (GameObject* gameObject)
{
	gameObject->Create();

	if (dynamic_cast<CollidingSphere*>(gameObject) != nullptr)
	{
		m_Primitives.push_back (dynamic_cast<CollidingSphere*>(gameObject));
		m_Colliders.push_back (dynamic_cast<CollidingSphere*>(gameObject));
	}
	else if (dynamic_cast<MassSpring*>(gameObject) != nullptr)
	{
		MassSpringUI::massSpringsList[m_MassSprings.size()] = gameObject->name;
		m_MassSprings.push_back (dynamic_cast<MassSpring*>(gameObject));
	}
	else if (dynamic_cast<Wind*>(gameObject) != nullptr)
	{
		m_Wind = dynamic_cast<Wind*>(gameObject);
	}
	else if (dynamic_cast<LightSource*>(gameObject) != nullptr)
	{
		m_LightSource = dynamic_cast<LightSource*>(gameObject);
	}else if (dynamic_cast<Primitive*>(gameObject) != nullptr)
	{
		m_Primitives.push_back (dynamic_cast<Primitive*>(gameObject));
	}
}

void Scene::UpdateCamera () { m_Camera->UpdateWithUI(); }

void Scene::Update ()
{
	if (m_Camera == nullptr) { throw std::runtime_error ("Camera not present!"); }

	UpdateCamera();
	UpdateGameObjects();
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
		massSpring->Update();
		UpdateTransform (massSpring);
		DrawGameObject (massSpring);
	}

	// Update Wind
	if (m_Wind != nullptr)
	{
		UpdateTransform (m_Wind);
		m_Wind->Update();
		DrawGameObject (m_Wind);
	}

	// Update Light
	if (m_LightSource != nullptr)
	{
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
