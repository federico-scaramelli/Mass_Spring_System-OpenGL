#pragma once
#include <map>
#include <string>
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
	void RemoveGameObject (const std::string& name);
	void EraseCollider (const std::string& name);
	
	[[nodiscard]] size_t GetGameObjectCount () const { return m_Primitives.size(); }
	
	std::map<std::string, GameObject*>& GetGameObjects () { return m_Primitives; }

	[[nodiscard]] Camera* GetCamera () const { return m_Camera; }
	[[nodiscard]] LightSource* GetLightSource () const { return m_LightSource; }
	[[nodiscard]] Wind* GetWind () const { return m_Wind; }
	[[nodiscard]] std::map<std::string, CollidingSphere*> GetColliders() const { return m_Colliders; }

	void Update ();


private:
	Scene () {}
	~Scene ();
	static Scene* instance;

	Camera* m_Camera;
	LightSource* m_LightSource;
	Wind* m_Wind;

	std::map<std::string, GameObject*> m_Primitives;
	std::vector<MassSpring*> m_MassSprings;
	std::map<std::string, CollidingSphere*> m_Colliders;
	
	void UpdateGameObjects ();

	void UpdateCamera ();

	void UpdateTransform (GameObject* gameObject);
	void DrawGameObject (GameObject* gameObject);

	friend class RendererUI;
};
