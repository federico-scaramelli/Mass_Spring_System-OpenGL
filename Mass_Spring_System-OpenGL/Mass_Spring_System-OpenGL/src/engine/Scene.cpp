#include "Scene.h"

#include <iostream>

#include "Camera.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Renderer.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/glm.hpp"
#include "../mass-spring/Cloth.h"

Scene::Scene(Renderer* renderer) : m_Renderer(renderer) {
	m_Renderer->AddListBoxUI("Select object", &selectedObject, sceneObjects, 2);
}

void Scene::AddCamera(Camera* camera) {
	m_Camera = camera;

	camera->GenerateUI(*m_Renderer);
	camera->GetTransform().SetPosition({ 0, 0, 100 });
}

void Scene::SetShaderUniforms(GameObject* object) {
	object->GetShader().Use();
	object->GetShader().SetUniform<glm::mat4>
		("projectionMatrix", m_Camera->GetProjectionMatrix());
}

void Scene::SetComputeShaderUniforms(GameObject* object) {
	object->GetComputeShader().Use();

	Cloth* cloth = dynamic_cast<Cloth*>(object);
	if (cloth != nullptr)
	{
		object->GetComputeShader().SetUniform<GLfloat>("deltaTime", cloth->m_Parameters.deltaTime);

		object->GetComputeShader().SetUniform<GLfloat>("elasticStiffness", cloth->m_Parameters.stiffness);

		object->GetComputeShader().SetUniform<GLfloat>("restLenHorizontal", cloth->m_Parameters.restLengthHorizontal);

		object->GetComputeShader().SetUniform<GLfloat>("restLenVertical", cloth->m_Parameters.restLengthVertical);

		object->GetComputeShader().SetUniform<GLfloat>("restLenDiagonal", cloth->m_Parameters.restLengthDiagonal);

		object->GetComputeShader().SetUniform<GLfloat>("particleMass", cloth->m_Parameters.particleMass);

		object->GetComputeShader().SetUniform<GLfloat>("damping", cloth->m_Parameters.damping);

		object->GetComputeShader().SetUniform<glm::vec4>("gravityAcceleration", cloth->m_Parameters.gravityAccel);
	}

}

void Scene::AddGameObject(GameObject* object) {
	m_GameObjects.push_back(object);

	SetShaderUniforms(object);

	if (object->GetComputeShader().GetID() != 0)
	{
		SetComputeShaderUniforms(object);
	}

	object->GenerateUI(*m_Renderer);
	sceneObjects[m_GameObjects.size() - 1] = object->name;
}

void Scene::AddLightSource(LightSource* light) {
	SetShaderUniforms(light);

	m_LightSource = light;

	light->GenerateUI(*m_Renderer);
}

void Scene::UpdateCamera() {
	m_Camera->UpdateWithUI();
}

void Scene::TransformLight() {
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4(1.f);

	m_LightSource->UpdateWithUI();

	modelViewMatrix = viewMatrix * m_LightSource->GetTransform().GetUpdatedModelMatrix();
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::mat3>(
		"normalMatrix", glm::inverseTranspose(glm::mat3(modelViewMatrix)));

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("lightPosition",
		m_LightSource->GetTransform().
		GetPosition());

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("lightAmbient",
		m_LightSource->GetAmbient());
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("light",
		m_LightSource->GetIntensity());

	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("matAmbient",
		m_LightSource->GetMesh().GetMaterial().
		GetAmbient());
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("matDiffuse",
		m_LightSource->GetMesh().GetMaterial().
		GetDiffuse());
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<glm::vec3>("matSpecular",
		m_LightSource->GetMesh().GetMaterial().
		GetSpecular());
	m_LightSource->GetMesh().GetMaterial().GetShader().SetUniform<GLfloat>("matShininess",
		m_LightSource->GetMesh().GetMaterial().GetShininess());
}

void Scene::UpdateLight() {
	m_LightSource->GetMesh().GetMaterial().GetShader().Use();
	TransformLight();
	m_Renderer->Draw(m_LightSource->GetMesh().m_vao, m_LightSource->GetMesh().m_indexBuffer,
		m_LightSource->GetMesh().GetMaterial().GetShader());
}

void Scene::Update() {
	if (m_Camera == nullptr) {
		throw std::runtime_error("Camera not present!");
	}

	UpdateCamera();

	if (m_LightSource != nullptr) {
		UpdateLight();
	}

	if (!m_GameObjects.empty() && !(selectedObject > m_GameObjects.size())) {
		UpdateGameObjects();
	}
}

void Scene::UpdateGameObjects() {
	// TODO: loop on game objects list to update all the active ones
	m_currentGameObject = m_GameObjects[selectedObject];

	//Update compute shader of game object
	if (m_currentGameObject->GetComputeShader().GetID() != 0)
	{
		m_currentGameObject->GetComputeShader().Use();
		m_currentGameObject->GetComputeShader().Compute();
		// m_currentGameObject->GetComputeShader().Compute(100, 10, 10);
		m_currentGameObject->GetComputeShader().Wait();

		// glMemoryBarrier(GL_BUFFER_UPDATE_BARRIER_BIT);
		// std::vector<Vertex> copy;
		// copy.resize(m_currentGameObject->GetMesh().GetVertices().size());
		// glad_glGetNamedBufferSubData(m_currentGameObject->GetMesh().m_vbo.GetID(),0, m_currentGameObject->GetMesh().m_vbo.GetSize(), copy.data());
		// std::cout << "( " << copy[0].position.x << " " << copy[0].position.y << " " <<  copy[0].position.z << " ) - " << copy[0].pinned.x << "\n";
		// glad_glGetNamedBufferSubData(m_currentGameObject->GetMesh().m_SecondaryVertexBuffer,0, m_currentGameObject->GetMesh().m_vbo.GetSize(), copy.data());
		// std::cout << "( " << copy[0].position.x << " " << copy[0].position.y << " " <<  copy[0].position.z << " ) - " << copy[0].pinned.x << "\n";
		// std::cout << " ----- \n";

		m_currentGameObject->GetMesh().SwapComputeBuffers();
	}

	m_currentGameObject->GetShader().Use();
	UpdateGameObject();

	DrawGameObject();
}

void Scene::UpdateGameObject() {
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4(1.f);

	m_currentGameObject->UpdateWithUI();

	// Light parameters loaded in object uniforms
	if (m_LightSource != nullptr) {
		m_currentGameObject->GetShader().SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);

		m_currentGameObject->GetShader().SetUniform<glm::vec3>("lightPosition",
			m_LightSource->GetTransform().
			GetPosition());

		m_currentGameObject->GetShader().SetUniform<glm::vec3>("lightAmbient",
			m_LightSource->GetAmbient());
		m_currentGameObject->GetShader().SetUniform<glm::vec3>("light",
			m_LightSource->GetIntensity());

		m_currentGameObject->GetShader().SetUniform<glm::vec3>("matAmbient",
			m_currentGameObject->GetMesh().GetMaterial().
			GetAmbient());
		m_currentGameObject->GetShader().SetUniform<glm::vec3>("matDiffuse",
			m_currentGameObject->GetMesh().GetMaterial().
			GetDiffuse());
		m_currentGameObject->GetShader().SetUniform<glm::vec3>("matSpecular",
			m_currentGameObject->GetMesh().GetMaterial().
			GetSpecular());
		m_currentGameObject->GetShader().SetUniform<GLfloat>("matShininess",
			m_currentGameObject->GetMesh().GetMaterial().GetShininess());
	}

	modelViewMatrix = viewMatrix * m_currentGameObject->GetTransform().GetUpdatedModelMatrix();
	m_currentGameObject->GetShader().SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
	m_currentGameObject->GetShader().SetUniform<glm::mat3>("normalMatrix",
		glm::inverseTranspose(glm::mat3(modelViewMatrix)));
}

void Scene::DrawGameObject() {
	m_Renderer->Draw(m_currentGameObject->GetMesh().m_vao,
		m_currentGameObject->GetMesh().m_indexBuffer,
		m_currentGameObject->GetShader());
}
