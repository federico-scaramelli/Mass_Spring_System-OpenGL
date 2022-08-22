#include "Transform.h"

#include "TransformUI.h"
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

void Transform::UpdateModelMatrix(glm::vec3 customRotation)
{
	modelMatrix = glm::mat4(1.0f);
	
	modelMatrix = glm::translate(modelMatrix, position);

	modelMatrix = glm::rotate (modelMatrix, glm::radians (customRotation.x), rightDirection);
	modelMatrix = glm::rotate (modelMatrix, glm::radians (customRotation.y), upDirection);
	modelMatrix = glm::rotate (modelMatrix, glm::radians (customRotation.z), forwardDirection);

	modelMatrix = glm::scale (modelMatrix, glm::vec3(scale));
}

void Transform::SetPosition (glm::vec3 newPosition)
{
	position = newPosition;
	transformUI->m_PositionData[0] = newPosition.x;
	transformUI->m_PositionData[1] = newPosition.y;
	transformUI->m_PositionData[2] = newPosition.z;
}

void Transform::AddPosition (const glm::vec3 position)
{
	SetPosition ({
		position.x + this->position.x,
		position.y + this->position.y,
		position.z + this->position.z
	});
}

void Transform::SetRotation (glm::vec3 newRotation)
{
	rotation = newRotation;
	transformUI->m_RotationData[0] = newRotation.x;
	transformUI->m_RotationData[1] = newRotation.y;
	transformUI->m_RotationData[2] = newRotation.z;
}

void Transform::AddRotation (glm::vec3 rotation)
{
	SetRotation ({
		fmod (rotation.x + this->rotation.x, 360.f),
		fmod (rotation.y + this->rotation.y, 360.f),
		fmod (rotation.z + this->rotation.z, 360.f)
	});
}

void Transform::SetUIObject (TransformUI* transformUI)
{
	this->transformUI = transformUI;
}

void Transform::UpdateWithUI ()
{
	UpdatePositionWithUI();
	UpdateRotationWithUI();
}

void Transform::UpdatePositionWithUI ()
{
	SetPosition ({ transformUI->m_PositionData[0], transformUI->m_PositionData[1], transformUI->m_PositionData[2] });
}

void Transform::UpdateRotationWithUI ()
{
	SetRotation ({ transformUI->m_RotationData[0], transformUI->m_RotationData[1], transformUI->m_RotationData[2] });
}