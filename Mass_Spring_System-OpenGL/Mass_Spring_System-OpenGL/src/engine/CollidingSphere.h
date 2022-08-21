#pragma once
#include "Primitive.h"

class CollidingSphere : public Primitive
{
public:
	CollidingSphere(const char* name, GLfloat radius) : Primitive(name, SPHERE, radius)
	{
		this->radius = radius;
	}

	GLuint radius;
};