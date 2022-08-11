#include "Scene.h"

#include <iostream>

#include "engine/Camera.h"
#include "engine/GameObject.h"
#include "engine/LightSource.h"
#include "engine/Renderer.h"
#include "glm/gtc/matrix_inverse.hpp"

Scene::Scene(Renderer* renderer) : m_Renderer(renderer)
{}

void Scene::AddCamera(Camera* camera)
{
	m_Camera = camera;
}

void Scene::AddGameObjectAndSetProj(GameObject* object)
{
	object->GetMaterial().m_Shader.Use();
	object->GetMaterial().m_Shader.SetUniform<glm::mat4>("projectionMatrix", m_Camera->GetProjectionMatrix());

	m_GameObjects.push_back(object);
}

void Scene::AddLightSource(LightSource* light)
{
	m_LightSource = light;
}

void Scene::ApplyTransformations()
{
	TransformCamera();
	TransformLight();
	TransformActiveObject();
}

void Scene::TransformCamera()
{
	m_Camera->GetTransform().SetRotation({cameraRotation[0], cameraRotation[1], cameraRotation[2]});
	m_Camera->GetTransform().SetPosition({cameraPosition[0], cameraPosition[1], cameraPosition[2]});
}

void Scene::TransformLight()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4(1.f);

	m_LightSource->GetTransform().SetPosition({lightPosition[0], lightPosition[1], lightPosition[2]});

	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightPosition",
	                                                            m_LightSource->GetTransform().GetPosition());

	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightAmbient",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightDiffuse",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightSpecular",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));

	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("matAmbient",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("matDiffuse",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::vec3>("matSpecular",
	                                                            glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
	m_LightSource->GetMaterial().m_Shader.SetUniform<GLfloat>("matShininess", 2);

	modelViewMatrix = viewMatrix * m_LightSource->GetTransform().GetUpdatedModelMatrix();
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
	m_LightSource->GetMaterial().m_Shader.SetUniform<glm::mat3>("normalMatrix",
	                                                            glm::inverseTranspose(glm::mat3(modelViewMatrix)));
}

void Scene::TransformActiveObject()
{
	glm::mat4 viewMatrix = m_Camera->GetUpdatedViewMatrix();
	glm::mat4 modelViewMatrix = glm::mat4(1.f);

	GameObject* activeObject = m_GameObjects[selectedObject];

	activeObject->GetTransform().SetPosition({objectPosition[0], objectPosition[1], objectPosition[2]});
	activeObject->GetTransform().SetRotation({objectRotation[0], objectRotation[1], objectRotation[2]});

	//light params loaded in object uniforms
	if(m_LightSource != nullptr)
	{
		activeObject->GetMaterial().m_Shader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);

		activeObject->GetMaterial().m_Shader.SetUniform<glm::mat3>("normalMatrix",
		                                                           glm::inverseTranspose(glm::mat3(modelViewMatrix)));

		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightPosition",
		                                                           m_LightSource->GetTransform().GetPosition());

		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightAmbient",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightDiffuse",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("lightSpecular",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));

		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("matAmbient",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("matDiffuse",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
		activeObject->GetMaterial().m_Shader.SetUniform<glm::vec3>("matSpecular",
		                                                           glm::vec3(lightColor[0], lightColor[1], lightColor[2]));
		activeObject->GetMaterial().m_Shader.SetUniform<GLfloat>("matShininess", 2);
	}
	


	modelViewMatrix = viewMatrix * activeObject->GetTransform().GetUpdatedModelMatrix();
	activeObject->GetMaterial().m_Shader.SetUniform<glm::mat4>("modelViewMatrix", modelViewMatrix);
	activeObject->GetMaterial().m_Shader.SetUniform<glm::mat3>("normalMatrix",
	                                                           glm::inverseTranspose(glm::mat3(modelViewMatrix)));
}

void Scene::Update()
{
	if(m_Camera == nullptr)
	{
		throw std::runtime_error("Camera not present!");
	}

	TransformCamera();

	if (m_LightSource != nullptr)
	{
		DrawLight();
	}


	if (!m_GameObjects.empty() && !(selectedObject > m_GameObjects.size()))
	{
		DrawActiveObject();
	}

	
}

void Scene::DrawLight()
{
	m_LightSource->GetMaterial().m_Shader.Use();
	TransformLight();
	m_Renderer->Draw(m_LightSource->GetMesh().vertexArrayObject, m_LightSource->GetMesh().indexBuffer,
	                 m_LightSource->GetMaterial().m_Shader);
}

void Scene::DrawActiveObject()
{
	GameObject* obj = m_GameObjects[selectedObject];

	obj->GetMaterial().m_Shader.Use();
	TransformActiveObject();

	m_Renderer->Draw(obj->GetMesh().vertexArrayObject, obj->GetMesh().indexBuffer, obj->GetMaterial().m_Shader);
}
