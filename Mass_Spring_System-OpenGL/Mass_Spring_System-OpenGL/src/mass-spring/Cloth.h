#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "../engine/GameObject.h"

#define LinearIndex(i, j, rowSize) ((i) * (rowSize) + (j))

struct Vertex;

class Cloth : public GameObject {
public:
	GLfloat m_Width;
	GLfloat m_Height;
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;

	PhysicsParameters m_Parameters;
	ShaderProgramCompute firstStageComputeShader{};
	ShaderProgramCompute secondStageComputeShader{};
	GLuint m_ComputeTempVertexBuffer;

	GLfloat stiffness = 1000000.f;
	GLfloat kSheering = 0.2f;
	GLfloat kBending = 0.2f;
	GLfloat restLengthHorizontal = 0.5f;
	GLfloat restLengthVertical = 0.5f;
	GLfloat restLengthDiagonal = 0.707f;
	GLfloat particleMass = 10;
	GLfloat clothMass = 1000;

	Cloth(GLfloat clothWidth, GLfloat clothHeight, GLint pointsWidth, GLint pointsHeight);

	void InitializeVertices();

	void InitializeIndices();

	void Create() override;
	void Update() override;

	void SetComputeBuffers();
	void BindComputeBuffers(int vboBind, int tempBind);

	void SwapComputeBuffers();

	glm::vec2 GetClothSize()
	{
		return {m_PointsByWidth, m_PointsByHeight};
	}
};
