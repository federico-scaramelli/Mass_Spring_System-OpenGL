#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(GLfloat aspect)
{
	SetPerspectiveProjection (FOV, aspect, NEAR_PLANE, FAR_PLANE);
}

void Camera::SetPerspectiveProjection (const GLfloat fovy, const GLfloat aspect, const GLfloat near, const GLfloat far)
{
	m_ProjectionMatrix = glm::perspective(glm::radians (fovy), aspect, near, far);
}

void Camera::UpdateViewMatrix ()
{
	glm::mat4 rotM = glm::mat4(1.0f);
	glm::mat4 transM;
	
	rotM = glm::rotate(rotM, glm::radians(GetTransform().GetPitch()), GetTransform().GetRightDirection());
	rotM = glm::rotate(rotM, glm::radians(GetTransform().GetYaw()), GetTransform().GetUpDirection());
	rotM = glm::rotate(rotM, glm::radians(GetTransform().GetRoll()), GetTransform().GetForwardDirection());

	glm::vec3 translation = GetTransform().GetPosition();
	
	transM = glm::translate(glm::mat4(1.0f), translation);
	
	m_ViewMatrix = rotM * transM;
}