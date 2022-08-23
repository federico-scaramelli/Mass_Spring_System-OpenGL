#pragma once

#include "MassSpring.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"


#define LinearIndex(i, j, rowSize) ((i) * (rowSize) + (j))

struct Vertex;
class ClothUI;

class Cloth : public MassSpring {
public:
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;
	GLfloat m_RestLengthHV;
	GLfloat m_RestLengthDiagonal;
	
	GLuint m_ComputeTempVertexBuffer;

	GLfloat correctionDumping=0.25f;
	GLfloat constraintDistanceMult = 1.1f;
	GLfloat selfCollisionDistanceMult = 0.8f;
	GLfloat sphereRepulsionDistMult = 1.05f;
	GLfloat sphereRepulsionDamping = 0.95f;
	
	Cloth(const char* name, uint16_t pointsByWidth, uint16_t pointsByHeight, float restLenghtHV);

	void InitializeVertices() override;
	void InitializeIndices() override;

	void Create() override;
	void Update() override;

	void SetComputeBuffers() override;
	void BindComputeBuffers(int vboBind, int tempBind) override;

	glm::vec2 GetClothSize()
	{
		return {m_PointsByWidth, m_PointsByHeight};
	}

	void PinAllEdges();
	void PinTopEdge();
	void PinLeftEdge();
	void PinLeftBorderVertices ();
	void PinCircleCenter();
	void PinCenter ();
	void PinTopPoints();
	void PinFourBorderPoints();

	void UpdateWithUI () override;

private:
	ClothUI* clothUI;
};
