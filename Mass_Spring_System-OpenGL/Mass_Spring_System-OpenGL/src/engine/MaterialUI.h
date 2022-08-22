#pragma once
#include "Renderer.h"
#include "glm/vec2.hpp"
#include "imgui/imgui.h"

class MaterialUI 
{
public:
	virtual ~MaterialUI () = default;
	virtual void Draw() = 0;
};
