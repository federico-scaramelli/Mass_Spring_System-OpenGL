#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

class Transform
{
private:
	glm::mat4 modelMatrix = glm::mat4 {1.f};
	glm::mat4 normalMatrix = glm::mat4 {1.f};

	glm::vec3 position{};
	glm::vec3 rotation{0,0,0};
	GLfloat scale = 1.f;

	glm::vec3 rightDirection{1,0,0};
	glm::vec3 upDirection{0,1,0};
	glm::vec3 forwardDirection{0,0,1};

public:
	inline static const glm::vec3 worldUpDirection {0.f, 1.f, 0.f};

#pragma region ModelMatrix

	glm::mat4 GetModelMatrix() { return modelMatrix; }
	
	glm::mat4 GetUpdatedModelMatrix()
	{
		UpdateModelMatrix();
		return modelMatrix;
	}

	void UpdateModelMatrix ();

#pragma endregion

#pragma region NormalMatrix

	glm::mat4 GetNormalMatrix() { return normalMatrix; }

#pragma endregion

#pragma region TransformManagement

	void SetPosition(glm::vec3 newPosition) { position = newPosition; }
	void AddPosition(const glm::vec3 position)
	{
		SetPosition ({
			position.x + this->position.x,
			position.y + this->position.y,
			position.z + this->position.z
		});
	}
	glm::vec3 GetPosition() { return position; }

	

	void SetRotation(glm::vec3 newRotation)
	{
		rotation = newRotation;
	}

	void AddRotation(glm::vec3 rotation)
	{
		SetRotation ({
			fmod (rotation.x + this->rotation.x, 360.f),
			fmod (rotation.y + this->rotation.y, 360.f),
			fmod (rotation.z + this->rotation.z, 360.f)
		});
	}

	glm::vec3 GetRotation() { return rotation; }
	GLfloat GetPitch() { return rotation.x; }
	GLfloat GetYaw() { return rotation.y; }
	GLfloat GetRoll() { return rotation.z; }

	void SetScale(GLfloat newScale) { scale = newScale; }
	GLfloat GetScale() { return scale; }

	glm::vec3 GetRightDirection() { return rightDirection; }
	glm::vec3 GetUpDirection() { return upDirection; }
	glm::vec3 GetForwardDirection() { return forwardDirection; }

#pragma endregion
};

