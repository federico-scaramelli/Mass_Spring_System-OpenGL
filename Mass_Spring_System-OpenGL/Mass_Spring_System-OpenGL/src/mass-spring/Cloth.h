#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "MassSpring.h"
#include "../engine/Primitive.h"

#define LinearIndex(i, j, rowSize) ((i) * (rowSize) + (j))

struct Vertex;

class Cloth : public MassSpring {
public:
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;
	GLfloat m_RestLengthHV;
	GLfloat m_RestLengthDiagonal;
	
	GLuint m_ComputeTempVertexBuffer;
	
	Cloth(uint16_t pointsByWidth, uint16_t pointsByHeight, float restLenghtHV);

	void InitializeVertices() override;
	void InitializeIndices() override;

	void Create() override;
	void Update() override;

	void UpdateCollidingSphereUniforms ();

	void SetComputeBuffers() override;
	void BindComputeBuffers(int vboBind, int tempBind) override;

	glm::vec2 GetClothSize()
	{
		return {m_PointsByWidth, m_PointsByHeight};
	}

	void PinAllEdges();
	void PinTopEdge();
	void PinCircleCenter();
	void PinCenter ();
	void PinTopPoints();
};
