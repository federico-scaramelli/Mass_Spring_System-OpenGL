#pragma once
#include "Transform.h"
#include "glad/glad.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Renderer.h"

#define FOV 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 5000.f

class Camera
{
private:
	Transform m_Transform;

	glm::mat4 m_ProjectionMatrix{1.f};
	glm::mat4 m_ViewMatrix{1.f};

public:
	Camera(GLfloat aspect);

	void SetPerspectiveProjection (const GLfloat fovy, const GLfloat aspect, const GLfloat near, const GLfloat far);

	void UpdateViewMatrix();

	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	glm::mat4& GetUpdatedViewMatrix()
	{
		UpdateViewMatrix();
		return m_ViewMatrix;
	}

	void UpdateWithUI()
	{
		m_Transform.UpdateWithUI();
	}

	void GenerateUI(Renderer& renderer)
	{
		m_Transform.GeneratePositionUI(renderer, -1000, 1000);
		m_Transform.GenerateRotationUI(renderer);
	}

	Transform& GetTransform() { return m_Transform; }
};