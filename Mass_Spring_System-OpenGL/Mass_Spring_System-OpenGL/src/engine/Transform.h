#pragma once
#include "Renderer.h"
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

	//Default opengl axis
	glm::vec3 rightDirection{1,0,0};
	glm::vec3 upDirection{0,1,0};
	glm::vec3 forwardDirection{0,0,-1};

	float UI_Position[3] = {0, 0, 0};
	float UI_Rotation[3] = {0, 0, 0};
	char UILabel_Position[40];
	char UILabel_Rotation[40];

	const char* name;

public:
	inline static const glm::vec3 worldUpDirection {0.f, 1.f, 0.f};

	Transform(const char* name) : name(name) {}
	Transform() = default;

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

	void SetPosition(glm::vec3 newPosition)
	{
		position = newPosition;
		UI_Position[0] = newPosition.x;
		UI_Position[1] = newPosition.y;
		UI_Position[2] = newPosition.z;
	}
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
		UI_Rotation[0] = newRotation.x;
		UI_Rotation[1] = newRotation.y;
		UI_Rotation[2] = newRotation.z;
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

	void SetRightDirection(glm::vec3 newDir) { rightDirection=newDir; }
	void SetUpDirection(glm::vec3 newDir) { upDirection=newDir; }
	void SetForwardDirection(glm::vec3 newDir) { forwardDirection=newDir; }
#pragma endregion

#pragma region Transform UI

	void GenerateUI(Renderer& renderer)
	{
		GeneratePositionUI (renderer);
		GenerateRotationUI (renderer);
	}

	void GeneratePositionUI(Renderer& renderer, float min = -250, float max = 250)
	{
		strcpy_s (UILabel_Position, name);
		strcat_s (UILabel_Position, " position");
		renderer.AddFloat3SliderUI (UILabel_Position, UI_Position, min, max);
	}

	void GenerateRotationUI(Renderer& renderer, float min = -180, float max = 180)
	{
		strcpy_s (UILabel_Rotation, name);
		strcat_s (UILabel_Rotation, " rotation");
		renderer.AddFloat3SliderUI (UILabel_Rotation, UI_Rotation, min, max);
	}

	void UpdateWithUI()
	{
		UpdatePositionWithUI();
		UpdateRotationWithUI();
	}

	void UpdatePositionWithUI()
	{
		SetPosition ({UI_Position[0], UI_Position[1], UI_Position[2]});
	}

	void UpdateRotationWithUI()
	{
		SetRotation ({UI_Rotation[0], UI_Rotation[1], UI_Rotation[2]});
	}

#pragma endregion
};