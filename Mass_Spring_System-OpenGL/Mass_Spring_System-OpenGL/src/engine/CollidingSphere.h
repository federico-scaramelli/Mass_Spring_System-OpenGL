#pragma once
#include "Primitive.h"

class CollidingSphere : public Primitive
{
public:

	CollidingSphere(const char* name, GLfloat size) : Primitive(name, SPHERE, size) {}
};