#pragma once
#include "GameObject.h"

// Primitive presets
enum PrimitiveType
{
	CUBE,
	SPHERE,
	CONE
};

class Primitive : public GameObject
{
public:	
	Primitive(const char* name, PrimitiveType type, GLuint size);
	Primitive(const char* name, PrimitiveType type, GLuint resolution, GLfloat radius, GLfloat height);

	void Create() override;
	void Update() override;
};