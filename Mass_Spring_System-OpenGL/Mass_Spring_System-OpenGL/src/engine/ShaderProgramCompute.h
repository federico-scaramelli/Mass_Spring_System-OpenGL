#pragma once

#include "ShaderProgram.h"
#include "glm/vec3.hpp"

class ShaderProgramCompute : public ShaderProgram {
private:
	int workGroupNumber = 1;
	glm::ivec3 workGroupSizes{10,1,1};//Fixed in comp shader

public:
	void Compute()
	{
		glDispatchCompute(workGroupSizes.x, workGroupSizes.y, workGroupSizes.z);
	}

	void Wait()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void SetWorkGroupSizes(glm::vec3 sizes)
	{
		workGroupSizes = sizes;
	}

	void SetWorkGroupNumberFromVertexNumber(int vertexNumber)
	{
		int totalInvocations=workGroupSizes.x * workGroupSizes.y * workGroupSizes.z;
		workGroupNumber = vertexNumber / totalInvocations;
	}
	
};
