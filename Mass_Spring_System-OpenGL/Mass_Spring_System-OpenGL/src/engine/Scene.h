#pragma once
#include <vector>

#include "Camera.h"
#include "GameObject.h"
#include "LightSource.h"
#include "Renderer.h"
class Renderer;
class LightSource;
class Camera;
class GameObject;

//Will be used as uniform
struct PhysicsParameters {
	GLfloat deltaTime = 0.016f;
	GLfloat stiffness = 1000.f;
	GLfloat restLengthHorizontal = 1.f;
	GLfloat restLengthVertical = 1.f;
	GLfloat restLengthDiagonal = 1.f;
	GLfloat useGravity = 1;
	GLfloat particleMass = 1;
	GLfloat damping=0.98f;
	glm::vec4 gravity {0.f,-9.81f,0.f, 0.f};
};

class Scene {
public:
	Scene(Renderer* renderer);
	
	const char* sceneObjects[10];
	int selectedObject = 0;

	void AddCamera(Camera* camera);
	void AddGameObject(GameObject* object);
	void AddLightSource(LightSource* light);

	void Update();
	

private:
	Renderer* m_Renderer;
	PhysicsParameters m_Parameters{};

	Camera* m_Camera;
	std::vector<GameObject*> m_GameObjects;
	GameObject* m_currentGameObject;
	LightSource* m_LightSource;

	void SetShaderUniforms (GameObject* object);
	void SetComputeShaderUniforms(GameObject* object);
	void UpdateGameObjects();

	void UpdateCamera();

	void TransformLight();
	void UpdateLight();

	void UpdateGameObject();
	void DrawGameObject ();
};
