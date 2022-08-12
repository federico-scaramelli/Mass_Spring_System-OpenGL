#include "Scene.h"

#include <iostream>

#include "Camera.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Renderer.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/glm.hpp"

Scene::Scene (Renderer* renderer) : m_Renderer (renderer)
{}

void Scene::AddCamera (Camera* camera)
{
	m_Camera = camera;
}

void Scene::SetProjectionMatrix (GameObject* object)
{
	object->GetMesh().GetMaterial().GetShader().Use();
	object->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat4>
		("projectionMatrix", m_Camera->GetProjectionMatrix());
}

void Scene::AddGameObject (GameObject* object)
{
	SetProjectionMatrix (object);

	m_GameObjects.push_back (object);
}

void Scene::AddLightSource (LightSource* light)
{
	SetProjectionMatrix (light);

	m_LightSource = light;
}

void Scene::UpdateCamera ()
{
	m_Camera->GetTransform().SetRotation ({ cameraRotation[0], cameraRotation[1], cameraRotation[2] });
	m_Camera->GetTransform().SetPosition ({ cameraPosition[0], cameraPosition[1], cameraPosition[2] });
}

void Scene::TransformLight ()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	m_LightSource->GetTransform().SetPosition ({ lightPosition[0], lightPosition[1], lightPosition[2] });

	modelViewMatrix = viewMatrix * m_LightSource->GetTransform().GetUpdatedModelMatrix();
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat3> (
		"normalMatrix", glm::inverseTranspose (glm::mat3 (modelViewMatrix)));

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightPosition",
	                                                                       m_LightSource->GetTransform().GetPosition());

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightAmbient",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightDiffuse",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightSpecular",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matAmbient",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matDiffuse",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matSpecular",
	                                                                       glm::vec3 (lightColor[0], lightColor[1],
		                                                                       lightColor[2]));
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<GLfloat> ("matShininess", 2);
}

void Scene::UpdateLight ()
{
	m_LightSource->GetMesh().GetMaterial().GetShader().Use();
	TransformLight();
	m_Renderer->Draw (m_LightSource->GetMesh().m_vao, m_LightSource->GetMesh().m_indexBuffer,
	                  m_LightSource->GetMesh().GetMaterial().GetShader());
}

void Scene::Update ()
{
	if (m_Camera == nullptr) {
		throw std::runtime_error ("Camera not present!");
	}

	UpdateCamera();

	if (m_LightSource != nullptr) {
		UpdateLight();
	}


	if (!m_GameObjects.empty() && !(selectedObject > m_GameObjects.size())) {
		UpdateGameObjects();
	}
}

void Scene::UpdateGameObjects ()
{
	// TODO: loop on game objects list to update all the active ones
	m_currentGameObject = m_GameObjects[selectedObject];
	m_currentGameObject->GetMesh().GetMaterial().GetShader().Use();

	UpdateGameObject();
	DrawGameObject();
}

void Scene::UpdateGameObject ()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4 (1.f);

	m_currentGameObject->GetTransform().SetPosition ({ objectPosition[0], objectPosition[1], objectPosition[2] });
	m_currentGameObject->GetTransform().SetRotation ({ objectRotation[0], objectRotation[1], objectRotation[2] });

	// Light parameters loaded in object uniforms
	if (m_LightSource != nullptr) {
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<
			glm::mat4> ("modelViewMatrix", modelViewMatrix);

		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat3> (
		  "normalMatrix", glm::inverseTranspose (glm::mat3 (modelViewMatrix)));

		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightPosition",
			m_LightSource->GetTransform().GetPosition());

		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightAmbient",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightDiffuse",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("lightSpecular",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));

		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matAmbient",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matDiffuse",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3> ("matSpecular",
			glm::vec3 (lightColor[0], lightColor[1],
			           lightColor[2]));
		m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<GLfloat> ("matShininess", 2);
	}


	modelViewMatrix = viewMatrix * m_currentGameObject->GetTransform().GetUpdatedModelMatrix();
	m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat4> ("modelViewMatrix", modelViewMatrix);
	m_currentGameObject->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat3> ("normalMatrix",
		glm::inverseTranspose (glm::mat3 (modelViewMatrix)));
}

void Scene::DrawGameObject ()
{
	m_Renderer->Draw (m_currentGameObject->GetMesh().m_vao,
	                  m_currentGameObject->GetMesh().m_indexBuffer,
	                  m_currentGameObject->GetMesh().GetMaterial().GetShader());
}
