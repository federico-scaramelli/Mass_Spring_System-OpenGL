#pragma once

#include "ShaderProgram.h"
#include "glm/vec3.hpp"

class ShaderProgramCompute : public ShaderProgram {
private:
	int workGroupNumber = 10;
	glm::ivec3 workGroupSize{1, 1, 1};

public:
	void Compute()
	{
		glDispatchCompute (workGroupSize.x, workGroupSize.y, workGroupSize.z);
	}

	void Wait()
	{
		glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
	}

	void SetWorkGroupNumber(int numberOfGroups)
	{
		workGroupNumber = numberOfGroups;
	}

	void SetWorkGroupSizeX(int size)
	{
		workGroupSize.x = size;
	}

	void SetWorkGroupSizeXFromTotalInvocation(int totalInvocations)
	{
		workGroupSize.x = totalInvocations / workGroupNumber;
	}
};
