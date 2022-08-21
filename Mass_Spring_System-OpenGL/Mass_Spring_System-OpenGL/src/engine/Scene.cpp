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

Scene::Scene (Renderer* renderer, PhysicsSolver* physicsSolver) : m_Renderer (renderer), m_PhysicsSolver (physicsSolver)
{
	m_Renderer->UI.SetScene (this);
}

void Scene::AddCamera (Camera* camera)
{
	m_Camera = camera;

	camera->GenerateUI();
	camera->GetTransform().SetPosition ({ 0, -100, 700 });
}

void Scene::SetShaderUniforms (GameObject* object)
{
	object->GetShader().Use();
	object->GetShader().SetUniform<glm::mat4>
		("projectionMatrix", m_Camera->GetProjectionMatrix());
}

void Scene::AddGameObject (GameObject* object)
{
	object->GetMaterial().fragShader = BlinnPhong;
	object->SetupGraphicsShader();

	auto massSpring = dynamic_cast<MassSpring*> (object);
	if (massSpring != nullptr)
	{
		MassSpringUI::sceneMassSprings[m_MassSprings.size()] = massSpring->name;
		m_MassSprings.push_back (massSpring);
		massSpring->SetComputeBuffers();
	}
	else
	{
		m_Primitives.push_back (object);
		auto sphere = dynamic_cast<CollidingSphere*> (object);
		if (sphere != nullptr) { m_PhysicsSolver->AddCollider (sphere); }
	}

	SetShaderUniforms (object);
	object->Create();
	object->GenerateUI();
}

void Scene::AddLightSource (LightSource* light)
{
	light->GetMaterial().fragShader = Solid;
	light->SetupGraphicsShader();
	SetShaderUniforms (light);

	m_LightSource = light;

	light->GenerateUI();
}

void Scene::AddWind (Wind* wind)
{
	wind->GetMaterial().fragShader = Solid;
	wind->SetupGraphicsShader();
	SetShaderUniforms (wind);

	m_Wind = wind;

	wind->GenerateUI();
}

void Scene::UpdateCamera () { m_Camera->UpdateWithUI(); }

void Scene::TransformLight ()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	m_LightSource->UpdateWithUI();

	modelViewMatrix = viewMatrix * m_LightSource->GetTransform().GetUpdatedModelMatrix();
	m_LightSource->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);

	m_LightSource->GetShader().SetUniform<glm::vec3> ("solidColor",
	                                                  { m_LightSource->GetIntensity() });
}

void Scene::UpdateLight ()
{
	m_LightSource->GetMesh().GetMaterial().GetShader().Use();
	TransformLight();
	m_Renderer->Draw (m_LightSource->GetMesh().m_vao, m_LightSource->GetMesh().m_indexBuffer,
	                  m_LightSource->GetMesh().GetMaterial().GetShader());
}

void Scene::TransformWind ()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	m_Wind->UpdateWithUI();

	modelViewMatrix = viewMatrix * m_Wind->GetTransform().GetUpdatedModelMatrix();
	m_Wind->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);

	m_Wind->GetShader().SetUniform<glm::vec3> ("solidColor",
	                                           { 0.6f, 0.8f, 0.9f });
}

void Scene::UpdateWind ()
{
	m_Wind->GetMesh().GetMaterial().GetShader().Use();
	TransformWind();
	m_Renderer->Draw (m_Wind->GetMesh().m_vao, m_Wind->GetMesh().m_indexBuffer,
	                  m_Wind->GetMesh().GetMaterial().GetShader());
}

void Scene::Update ()
{
	if (m_Camera == nullptr) { throw std::runtime_error ("Camera not present!"); }

	UpdateCamera();

	if (m_LightSource != nullptr) { UpdateLight(); }

	if (m_Wind != nullptr) { UpdateWind(); }

	if (!m_Primitives.empty() || !m_MassSprings.empty()) { UpdateGameObjects(); }

	m_PhysicsSolver->Update();
}

void Scene::UpdateGameObjects ()
{
	for (auto primitive : m_Primitives)
	{
		if (!primitive->m_IsActive) continue;

		// Custom update behavior of game object
		primitive->Update();

		UpdateGameObject (primitive);

		DrawGameObject (primitive);
	}

	auto massSpring = m_MassSprings[MassSpringUI::selectedMassSpring];
	m_PhysicsSolver->SetActiveMassSpring (massSpring);
	massSpring->Update();
	UpdateGameObject (massSpring);
	DrawGameObject (massSpring);
}

void Scene::UpdateGameObject (GameObject* gameObject)
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	gameObject->GetShader().Use();
	gameObject->UpdateWithUI();

	// Light parameters loaded in object uniforms
	if (m_LightSource != nullptr)
	{
		gameObject->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);

		gameObject->GetShader().SetUniform<glm::vec3> ("lightPosition",
		                                               m_LightSource->GetTransform().
		                                                              GetPosition());

		gameObject->GetShader().SetUniform<glm::vec3> ("lightAmbient",
		                                               m_LightSource->GetAmbient());
		gameObject->GetShader().SetUniform<glm::vec3> ("lightDiffuse",
		                                               m_LightSource->GetIntensity());

		gameObject->GetShader().SetUniform<glm::vec3> ("matAmbient",
		                                               gameObject->GetMesh().GetMaterial().
		                                                           GetAmbient());
		gameObject->GetShader().SetUniform<glm::vec3> ("matDiffuse",
		                                               gameObject->GetMesh().GetMaterial().
		                                                           GetDiffuse());
		gameObject->GetShader().SetUniform<glm::vec3> ("matSpecular",
		                                               gameObject->GetMesh().GetMaterial().
		                                                           GetSpecular());
		gameObject->GetShader().SetUniform<GLfloat> ("matShininess",
		                                             gameObject->GetMesh().GetMaterial().GetShininess());
	}

	modelViewMatrix = viewMatrix * gameObject->GetTransform().GetUpdatedModelMatrix();
	gameObject->GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);
	gameObject->GetShader().SetUniform<glm::mat3> ("normalMatrix",
	                                               glm::inverseTranspose (glm::mat3 (modelViewMatrix)));
}

void Scene::DrawGameObject (GameObject* gameObject)
{
	m_Renderer->Draw (gameObject->GetMesh().m_vao,
	                  gameObject->GetMesh().m_indexBuffer,
	                  gameObject->GetShader());
}
