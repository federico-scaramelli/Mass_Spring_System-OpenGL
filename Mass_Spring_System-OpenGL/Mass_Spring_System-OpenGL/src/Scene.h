#pragma once
#include <vector>

#include "engine/Camera.h"
#include "engine/GameObject.h"
#include "engine/LightSource.h"
#include "engine/Renderer.h"
class Renderer;
class LightSource;
class Camera;
class GameObject;

class Scene {
public:
	//UIElements
	float cameraPosition[3] = {0, 0, 100.f};
	float cameraRotation[3] = {0, 0, 0};
	float lightPosition[3] = {-10, -10, 25};
	float lightColor[3] = {0, 0.4f, 0.6f};
	float objectPosition[3] = {0, 0, 0};
	float objectRotation[3] = {0, 0, 0};

	const char* sceneObjects[2]{"Cloth", "Rope"};
	int selectedObject = 0;


	Renderer* m_Renderer;

	int activeCamera = 0;
	int activeLight = 0;
	// int activeObject = 0;

	Camera* m_Camera;
	std::vector<GameObject*> m_GameObjects;
	LightSource* m_LightSource;

	Scene(Renderer* renderer);

	void AddCamera(Camera* camera);
	void AddGameObjectAndSetProj(GameObject* object);
	void AddLightSource(LightSource* light);

	void ApplyTransformations();
	void TransformCamera();
	void TransformLight();
	void TransformActiveObject();

	void Update();
	void DrawLight();
	void DrawActiveObject();
};
