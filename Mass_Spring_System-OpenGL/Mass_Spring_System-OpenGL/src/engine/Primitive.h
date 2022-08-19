#pragma once
#include "GameObject.h"

enum PrimitiveType
{
	CUBE,
	SPHERE
};

class Primitive : public GameObject
{
public:
	float size = 1;

	Primitive(const char* name, PrimitiveType type, float size);
	void Create() override;
	void Update() override;
};