#pragma once
#include "Primitive.h"

// Spherical primitive trated as a spherical mesh with collider
class CollidingSphere : public Primitive
{
public:
	CollidingSphere(const char* name, GLfloat radius) : Primitive(name, SPHERE, radius)
	{
		this->radius = radius;
	}

	GLuint radius;
};