#pragma once
#include <vector>

class Wind;
class MassSpring;
class PhysicsSolver;
class CollidingSphere;
class Renderer;
class LightSource;
class Camera;
class GameObject;

class Scene
{
public:
	Scene (Renderer* renderer, PhysicsSolver* physicsSolver);

	void AddCamera (Camera* camera);
	void AddGameObject (GameObject* object);
	void AddLightSource (LightSource* light);
	void AddWind (Wind* wind);

	[[nodiscard]] size_t GetGameObjectCount () const { return m_Primitives.size(); }

	std::vector<GameObject*>& GetGameObjects () { return m_Primitives; }

	[[nodiscard]] Camera* GetCamera() const { return m_Camera; }
	[[nodiscard]] LightSource* GetLightSource() const { return m_LightSource; }
	[[nodiscard]] Wind* GetWind() const { return m_Wind; }
	[[nodiscard]] Renderer* GetRenderer() const { return m_Renderer; }

	void Update ();


private:
	Renderer* m_Renderer;
	PhysicsSolver* m_PhysicsSolver;

	Camera* m_Camera;
	LightSource* m_LightSource;
	Wind* m_Wind;

	std::vector<GameObject*> m_Primitives;
	std::vector<MassSpring*> m_MassSprings;

	void SetShaderUniforms (GameObject* object);
	void UpdateGameObjects ();

	void UpdateCamera ();

	void TransformLight ();
	void UpdateLight ();

	void TransformWind();
	void UpdateWind();

	void UpdateGameObject (GameObject* gameObject);
	void DrawGameObject (GameObject* gameObject);
	
	friend class RendererUI;
};
