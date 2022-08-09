#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"

void Transform::UpdateModelMatrix ()
{
	modelMatrix = glm::mat4(1.0f);
	
	modelMatrix = glm::translate(modelMatrix, position);

	modelMatrix = glm::rotate (modelMatrix, glm::radians (GetPitch()), rightDirection);
	modelMatrix = glm::rotate (modelMatrix, glm::radians (GetYaw()), upDirection);
	modelMatrix = glm::rotate (modelMatrix, glm::radians (GetRoll()), forwardDirection);

	modelMatrix = glm::scale (modelMatrix, glm::vec3(scale));
}

void Transform::UpdateDirectionAxis ()
{
    forwardDirection.x = cos(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
    forwardDirection.y = sin(glm::radians(GetPitch()));
    forwardDirection.z = sin(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
    forwardDirection = glm::normalize(forwardDirection);
	
    rightDirection = glm::normalize(glm::cross(forwardDirection, worldUpDirection));
    upDirection = glm::normalize(glm::cross(rightDirection, forwardDirection));
}
