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
#include "Utils.h"

// Cleanup of the dynamically allocated objects
Scene::~Scene ()
{
	delete instance;
	delete m_Camera;
	delete m_LightSource;
	delete m_Wind;
	Utils::destroyMap (m_Primitives);
	Utils::destroyMap (m_MassSprings);
}

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
	// Custom create method of specific type of GameObject
	gameObject->Create();

	// Different behavior based on the class of the GameObject.
	// Mainly to maintain dedicated data structures for each type.
	if (dynamic_cast<CollidingSphere*>(gameObject) != nullptr)
	{
		m_Primitives.insert ({gameObject->name, dynamic_cast<CollidingSphere*>(gameObject)});
		m_Colliders.insert ({gameObject->name, dynamic_cast<CollidingSphere*>(gameObject)});
	}
	else if (dynamic_cast<MassSpring*>(gameObject) != nullptr)
	{
		MassSpringUI::massSpringsList[m_MassSprings.size()] = gameObject->name.c_str();
		m_MassSprings.insert ({gameObject->name, dynamic_cast<MassSpring*>(gameObject)});
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
		m_Primitives.insert ({gameObject->name, dynamic_cast<Primitive*>(gameObject)});
	}
}

void Scene::RemoveGameObject (const std::string& name)
{
	auto gameObject = m_Primitives.find (name)->second;
	m_Primitives.erase (name);
	delete gameObject;
}

// Convenient method to reset the current selected Mass Spring object
void Scene::ResetMassSpring ()
{
	std::string selectedName = MassSpringUI::massSpringsList[MassSpringUI::selectedMassSpring];
	auto oldObj = m_MassSprings.at (selectedName);
	oldObj->Reset();
}

// Convenient method to delete the selected Collider object
void Scene::EraseCollider (const std::string& name)
{
	m_Colliders.erase (name);
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
	for (auto [name, primitive] : m_Primitives)
	{
		if (!primitive->m_IsActive) continue;

		// Custom update behavior of game object
		primitive->Update();

		UpdateTransform (primitive);
		DrawGameObject (primitive);
	}
	
	// Update selected Mass Spring
	if (!m_MassSprings.empty())
	{
		std::string selectedName = MassSpringUI::massSpringsList[MassSpringUI::selectedMassSpring];
		auto massSpring = m_MassSprings.at (selectedName);
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
	// Update material and all the values related to the UI
	gameObject->GetMaterial().Update();
	gameObject->UpdateWithUI();

	// Compute transform matrices and set the model view to the uniform
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);
	modelViewMatrix = viewMatrix * gameObject->GetTransform().GetUpdatedModelMatrix();
	gameObject->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);

	// Set the normalMatrix uniform only for shaders that use it
	if (gameObject->GetMaterial().fragShader == BlinnPhong || gameObject->GetMaterial().fragShader == Normal)
	{
		gameObject->GetShader().SetUniform<glm::mat3> ("normalMatrix",
													  glm::inverseTranspose (glm::mat3 (modelViewMatrix)));
	}
}

void Scene::DrawGameObject (GameObject* gameObject)
{
	// Draw the gameobject using its vao, index buffer and shader program
	Renderer::GetInstance()->Draw (gameObject->GetMesh().m_vao,
	                  gameObject->GetMesh().m_indexBuffer,
	                  gameObject->GetShader());
}

Scene* Scene::instance = nullptr;
