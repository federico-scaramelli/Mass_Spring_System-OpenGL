#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include "../engine/GameObject.h"

#define LinearIndex(i, j, rowSize) ((i) * (rowSize) + (j))

struct Vertex;

//Will be used as uniform
struct PhysicsParameters {
	const GLfloat deltaTime = 0.016f;
	const uint16_t subSteps = 32;
	const GLfloat subStepDt = deltaTime / static_cast<float>(subSteps);

	const GLfloat damping = 0.98f;
	//const glm::vec4 gravityAccel{ 0.f, - 9.81f, 0.f, 0.f };
	const glm::vec4 gravityAccel{ 0.f, -200, 0.f, 0.f };

	GLfloat stiffness = 10.f;
	GLfloat kSheering = 1.f;
	GLfloat kBending = kSheering * 0.2f;
	GLfloat particleMass = 1.f;
};

class Cloth : public GameObject {
public:
	GLint m_PointsByWidth;
	GLint m_PointsByHeight;
	GLfloat m_RestLengthHV;

	PhysicsParameters m_Parameters;
	ShaderProgramCompute firstStageComputeShader{};
	ShaderProgramCompute secondStageComputeShader{};
	GLuint m_ComputeTempVertexBuffer;

	GLfloat restLengthDiagonal;
	
	GLfloat clothMass;

	Cloth(uint16_t pointsByWidth, uint16_t pointsByHeight, float restLenghtHV);

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

	void PinAllEdges();
	void PinTopEdge();
	void PinCenter();
	void PinTopPoints();
};
