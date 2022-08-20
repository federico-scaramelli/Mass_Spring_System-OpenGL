#pragma once
#include <vector>


class GameObject;
class Scene;

class RendererUI
{
public:
	RendererUI() = default;

	void SetScene(Scene* scene)
	{
		this->scene = scene;
	}

	void DrawUI ();

private:
	Scene* scene;
};
