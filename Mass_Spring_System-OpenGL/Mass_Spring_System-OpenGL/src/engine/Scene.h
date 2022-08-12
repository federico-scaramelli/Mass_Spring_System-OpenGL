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
	
	const char* sceneObjects[10];
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
