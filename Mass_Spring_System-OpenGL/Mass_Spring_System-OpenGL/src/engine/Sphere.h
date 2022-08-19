#pragma once
#include "Primitive.h"
#include "SphereCollider.h"

class Sphere : public Primitive
{
public:
	SphereCollider collider;

	Sphere(const char* name, GLfloat size) : Primitive(name, SPHERE, size)
	{
		collider = {size, GetTransform().GetPosition()};
	}

	void Update() override
	{
		collider.centerPosition = GetTransform().GetPosition();
	}
};