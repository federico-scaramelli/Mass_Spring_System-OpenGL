#pragma once
#include <vector>
#include "../mass-spring/PhysicsSolver.h"

class Primitive;
class Wind;
class MassSpring;
class CollidingSphere;
class LightSource;
class Camera;
class GameObject;

class Scene
{
public:
	Scene (Scene& other) = delete;
	Scene operator= (const Scene&) = delete;

	static Scene* GetInstance ();

	void AddCamera (Camera* camera);
	void AddGameObject (MassSpring* massSpring);
	void AddGameObject (Primitive* primitive);
	void AddGameObject (CollidingSphere* collidingSphere);
	void AddGameObject (Wind* wind);
	void AddGameObject (LightSource* light);

	[[nodiscard]] size_t GetGameObjectCount () const { return m_Primitives.size(); }

	std::vector<GameObject*>& GetGameObjects () { return m_Primitives; }

	[[nodiscard]] Camera* GetCamera () const { return m_Camera; }
	[[nodiscard]] LightSource* GetLightSource () const { return m_LightSource; }
	[[nodiscard]] Wind* GetWind () const { return m_Wind; }

	void Update ();


private:
	Scene () : m_PhysicsSolver() {}
	~Scene () { delete instance; }
	static Scene* instance;

	PhysicsSolver m_PhysicsSolver;

	Camera* m_Camera;
	LightSource* m_LightSource;
	Wind* m_Wind;

	std::vector<GameObject*> m_Primitives;
	std::vector<MassSpring*> m_MassSprings;
	
	void UpdateGameObjects ();

	void UpdateCamera ();

	void UpdateTransform (GameObject* gameObject);
	void DrawGameObject (GameObject* gameObject);

	friend class RendererUI;
};
