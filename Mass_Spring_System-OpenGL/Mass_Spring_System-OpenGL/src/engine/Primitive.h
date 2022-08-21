#pragma once
#include "GameObject.h"

enum PrimitiveType
{
	CUBE,
	SPHERE,
	CONE
};

class Primitive : public GameObject
{
public:
	//float size = 1;
	
	Primitive(const char* name, PrimitiveType type, GLuint size);
	Primitive(const char* name, PrimitiveType type, GLuint resolution, GLfloat radius, GLfloat height);

	void Create() override;
	void Update() override;

	void BuildSphereMesh(Mesh& mesh, GLfloat radius);
};