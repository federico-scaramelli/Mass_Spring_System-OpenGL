#include "Primitive.h"

void BuildCubeMesh(Mesh& mesh, float size)
{
	auto& vertices = mesh.GetVertices();
	vertices.clear();

	std::vector<Vertex> cubeVertices = {
		{{-size * 0.5f, -size * 0.5f, size * 0.5f, 0.f}},
		{{size * 0.5f, -size * 0.5f, size * 0.5f, 0.f}},
		{{size * 0.5f, size * 0.5f, size * 0.5f, 0.f}},
		{{-size * 0.5f, size * 0.5f, size * 0.5f, 0.f}},
		{{-size * 0.5f, -size * 0.5f, -size * 0.5f, 0.f}},
		{{size * 0.5f, -size * 0.5f, -size * 0.5f, 0.f}},
		{{size * 0.5f, size * 0.5f, -size * 0.5f, 0.f}},
		{{-size * 0.5f, size * 0.5f, -size * 0.5f, 0.f}}
	};

	vertices.assign(cubeVertices.begin(), cubeVertices.end());

	auto& indices = mesh.GetIndices();
	indices = {
		//front
		0, 1, 2,
		2, 3, 0,

		//right
		1, 5, 6,
		6, 2, 1,

		//left
		0, 3, 7,
		7, 4, 0,

		//up
		3, 2, 6,
		6, 7, 3,

		//down
		0, 4, 5,
		5, 1, 0,

		//back 
		5, 4, 7,
		7, 6, 5
	};
}

void BuildSphereMesh(Mesh& mesh, float radius)
{
	//Vertici
	auto& vertices = mesh.GetVertices();
	vertices.clear();

	float PI = 3.14f;
	int stackCount = 8;
	int sectorCount = stackCount * 3;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);          // r * cos(u)
		z = radius * sinf(stackAngle);           // r * sin(u)

		for (int j = 0; j <= sectorCount; ++j)
		{
			Vertex vertex{};

			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

			vertex.position = { x,y,z, 0 };

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			vertex.normal = { nx,ny,nz, 0 };

			vertices.push_back(vertex);
		}
	}

	//Indici
	auto& indices = mesh.GetIndices();
	indices.clear();

	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
}

Primitive::Primitive (const char* name, PrimitiveType type, float size) : GameObject (name)
{
	this->size = size;

	switch (type)
	{
	case CUBE:
	  BuildCubeMesh (m_Mesh, size);
	  break;
	case SPHERE:
	  BuildSphereMesh (m_Mesh, size);
	  break;
	}
}

void Primitive::Create() {}
void Primitive::Update() {}