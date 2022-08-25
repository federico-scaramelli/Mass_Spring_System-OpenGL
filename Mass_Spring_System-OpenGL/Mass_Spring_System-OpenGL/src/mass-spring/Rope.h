#pragma once

#include "glad/glad.h"
#include "glm/vec3.hpp"

#include <array>
#include <utility>
#include "MassSpring.h"

struct Node {
	glm::vec4 position;
	glm::vec4 velocity;
	glm::vec4 oldPosition;
	glm::vec4 pinned;
};

class Rope : public MassSpring {
private:
	GLfloat m_RestLength;
	GLuint m_PointsByLength;
	GLfloat m_Radius;

	std::vector<Node> m_Nodes;

	GLuint m_ComputeNodesInBuffer;
	GLuint m_ComputeNodesOutBuffer;

	std::array<glm::vec2, 8> vertexPositionsWeigths = {
		{
			{1.f,	  0.f},
			{0.7f,	  0.7f},
			{0.f,	  1.f},
			{-0.7f,	  0.7f},
			{-1.f,	  0.f},
			{-0.7f,	  -0.7f},
			{0.f,	  -1.f},
			{0.7f,	  -0.7f}
		}
	};

public:
	Rope(GLuint pointsByLength, uint16_t restLenght, GLfloat radius);

	void InitializeNodes();

	void InitializeVertices() override;

	void InitializeIndices() override;

	void CreateFrontSurfaceIndices();

	void CreateMidSurfaceIndices();

	void CreateBackSurfaceIndices();

	void SetComputeBuffers() override;
	void BindComputeBuffers(int vboBind, int tempBind) override {}
	
	void Create() override;
	void Update() override;
	void Reset () override;
};
