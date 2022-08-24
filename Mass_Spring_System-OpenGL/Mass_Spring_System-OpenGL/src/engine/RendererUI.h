#pragma once
#include "glad/glad.h"

class RendererUI
{
private:
	GLfloat newSphereRadius = 25;
	GLuint newSphereCount = 0;

	void AddSphereUI();

public:
	RendererUI() = default;

	void DrawUI ();
};
