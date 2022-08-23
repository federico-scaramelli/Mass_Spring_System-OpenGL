#pragma once
#include <vector>

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
	void AddGameObject (GameObject* gameObject);

	[[nodiscard]] size_t GetGameObjectCount () const { return m_Primitives.size(); }

	std::vector<GameObject*>& GetGameObjects () { return m_Primitives; }

	[[nodiscard]] Camera* GetCamera () const { return m_Camera; }
	[[nodiscard]] LightSource* GetLightSource () const { return m_LightSource; }
	[[nodiscard]] Wind* GetWind () const { return m_Wind; }
	[[nodiscard]] std::vector<CollidingSphere*> GetColliders() const { return m_Colliders; }

	void Update ();


private:
	Scene () {}
	~Scene () { delete instance; }
	static Scene* instance;

	Camera* m_Camera;
	LightSource* m_LightSource;
	Wind* m_Wind;

	std::vector<GameObject*> m_Primitives;
	std::vector<MassSpring*> m_MassSprings;
	std::vector<CollidingSphere*> m_Colliders;
	
	void UpdateGameObjects ();

	void UpdateCamera ();

	void UpdateTransform (GameObject* gameObject);
	void DrawGameObject (GameObject* gameObject);

	friend class RendererUI;
};
