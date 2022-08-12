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

class Scene {
public:
	Scene(Renderer* renderer);

	//UIElements
	float cameraPosition[3] = {0, 0, 100.f};
	float cameraRotation[3] = {0, 0, 0};
	float lightPosition[3] = {-10, -10, 25};
	float lightColor[3] = {0, 0.4f, 0.6f};
	float objectPosition[3] = {0, 0, 0};
	float objectRotation[3] = {0, 0, 0};

	const char* sceneObjects[2]{"Cloth", "Rope"};
	int selectedObject = 0;

	void AddCamera(Camera* camera);
	void AddGameObject(GameObject* object);
	void AddLightSource(LightSource* light);

	void Update();
	

private:
	Renderer* m_Renderer;

	Camera* m_Camera;
	std::vector<GameObject*> m_GameObjects;
	GameObject* m_currentGameObject;
	LightSource* m_LightSource;

	void SetProjectionMatrix (GameObject* object);
	void UpdateGameObjects();

	void UpdateCamera();

	void TransformLight();
	void UpdateLight();

	void UpdateGameObject();
	void DrawGameObject ();
};
