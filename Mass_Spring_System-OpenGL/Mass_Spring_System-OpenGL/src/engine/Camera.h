#pragma once
#include "Transform.h"
#include "glad/glad.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "TransformUI.h"

#define FOV 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 5000.f

class Camera
{
private:
	Transform m_Transform;
	TransformUI* m_TransformUI;

	glm::mat4 m_ProjectionMatrix{1.f};
	glm::mat4 m_ViewMatrix{1.f};

public:
	Camera(GLfloat aspect);
	~Camera();

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

	void GenerateUI()
	{
		m_Transform.SetUIObject (m_TransformUI);
		m_TransformUI->SetPositionRange ({ -1000, 1000});
	}

	TransformUI* GetUI() { return m_TransformUI; }

	Transform& GetTransform() { return m_Transform; }
};