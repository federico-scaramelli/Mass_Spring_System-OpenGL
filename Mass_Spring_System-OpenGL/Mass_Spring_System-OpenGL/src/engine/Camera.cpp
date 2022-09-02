#include "Camera.h"

#include "TransformUI.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(GLfloat aspect) 
{
	SetPerspectiveProjection(FOV, aspect, NEAR_PLANE, FAR_PLANE);
	m_Transform = {"Camera"};
	m_TransformUI = new TransformUI("Camera");
}

Camera::~Camera ()
{
	delete m_TransformUI;
}

void Camera::SetPerspectiveProjection(const GLfloat fovy, const GLfloat aspect, const GLfloat near, const GLfloat far)
{
	Camera::m_ProjectionMatrix = glm::perspective(glm::radians(fovy), aspect, near, far);
}

void Camera::UpdateViewMatrix()
{
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	glm::mat4 transMatrix = glm::mat4(1.0f);
	
	rotMatrix = glm::rotate(rotMatrix, glm::radians(GetTransform().GetPitch()), GetTransform().GetRightDirection());
	rotMatrix = glm::rotate(rotMatrix, glm::radians(GetTransform().GetYaw()), GetTransform().GetUpDirection());
	rotMatrix = glm::rotate(rotMatrix, glm::radians(GetTransform().GetRoll()), GetTransform().GetForwardDirection());
	
	glm::vec3 translation = -1.f * GetTransform().GetPosition();
	transMatrix = glm::translate(transMatrix, translation);
	
	m_ViewMatrix = rotMatrix * transMatrix;
}
