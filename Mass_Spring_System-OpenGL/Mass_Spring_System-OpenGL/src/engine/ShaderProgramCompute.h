#pragma once

#include "ShaderProgram.h"
#include "glm/vec3.hpp"

class ShaderProgramCompute : public ShaderProgram {
private:
	int numWorkGroups = 1;
	glm::ivec3 workGroupSize{10, 1, 1}; //Fixed in comp shader

public:
	void Compute()
	{
		glDispatchCompute(workGroupSize.x, workGroupSize.y, workGroupSize.z);
	}

	void Wait()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void SetWorkGroupSize(glm::vec3 size)
	{
		workGroupSize = size;
	}

	void SetWorkGroupNumberFromVertexNumber(int vertexNumber)
	{
		int totalInvocationsPerGroup = workGroupSize.x * workGroupSize.y * workGroupSize.z;
		numWorkGroups = vertexNumber / totalInvocationsPerGroup;
	}

};
