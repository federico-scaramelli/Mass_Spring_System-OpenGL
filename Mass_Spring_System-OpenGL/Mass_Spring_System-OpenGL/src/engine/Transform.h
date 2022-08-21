#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

class TransformUI;

class Transform
{
private:
	glm::mat4 modelMatrix = glm::mat4{ 1.f };
	glm::mat4 normalMatrix = glm::mat4{ 1.f };

	glm::vec3 position{};
	glm::vec3 rotation{ 0, 0, 0 };
	GLfloat scale = 1.f;

	//Default opengl axis
	glm::vec3 rightDirection{ 1, 0, 0 };
	glm::vec3 upDirection{ 0, 1, 0 };
	glm::vec3 forwardDirection{ 0, 0, -1 };

	TransformUI* transformUI = nullptr;

	const char* name;

public:
	inline static const glm::vec3 worldUpDirection{ 0.f, 1.f, 0.f };

	Transform (const char* name) : name (name) {}
	Transform () = default;

#pragma region ModelMatrix

	glm::mat4 GetModelMatrix () { return modelMatrix; }

	glm::mat4 GetUpdatedModelMatrix ()
	{
		UpdateModelMatrix();
		return modelMatrix;
	}

	glm::mat4 GetUpdatedCustomRotationModelMatrix (glm::vec3 customRotation)
	{
		UpdateModelMatrix(customRotation);
		return modelMatrix;
	}

	void UpdateModelMatrix ();

	void UpdateModelMatrix (glm::vec3 customRotation);

#pragma endregion

#pragma region NormalMatrix

	glm::mat4 GetNormalMatrix () { return normalMatrix; }

#pragma endregion

#pragma region TransformManagement

	void SetPosition (glm::vec3 newPosition);

	void AddPosition (const glm::vec3 position);

	glm::vec3 GetPosition () { return position; }

	void SetRotation (glm::vec3 newRotation);

	void AddRotation (glm::vec3 rotation);

	glm::vec3 GetRotation () { return rotation; }
	GLfloat GetPitch () { return rotation.x; }
	GLfloat GetYaw () { return rotation.y; }
	GLfloat GetRoll () { return rotation.z; }

	void SetScale (GLfloat newScale) { scale = newScale; }
	GLfloat GetScale () { return scale; }

	glm::vec3 GetRightDirection () { return rightDirection; }
	glm::vec3 GetUpDirection () { return upDirection; }
	glm::vec3 GetForwardDirection () { return forwardDirection; }

	void SetRightDirection (glm::vec3 newDir) { rightDirection = newDir; }
	void SetUpDirection (glm::vec3 newDir) { upDirection = newDir; }
	void SetForwardDirection (glm::vec3 newDir) { forwardDirection = newDir; }
#pragma endregion

#pragma region Transform UI

	void GenerateUI (TransformUI* transformUI);

	void UpdateWithUI ();

	void UpdatePositionWithUI ();

	void UpdateRotationWithUI ();

#pragma endregion
};
