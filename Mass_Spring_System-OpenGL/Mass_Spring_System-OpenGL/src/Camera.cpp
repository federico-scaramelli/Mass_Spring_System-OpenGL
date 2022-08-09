#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

void Camera::SetPerspectiveProjection (const GLfloat fovy, const GLfloat aspect, const GLfloat near, const GLfloat far)
{
	m_ProjectionMatrix = glm::perspective(glm::radians (fovy), aspect, near, far);

}

void Camera::UpdateView ()
{
	m_ViewMatrix = glm::lookAt (m_Transform.GetPosition(),
								m_Transform.GetPosition() + m_Transform.GetForwardDirection(),  
								m_Transform.GetUpDirection());
}