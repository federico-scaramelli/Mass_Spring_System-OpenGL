#pragma once

#include "ShaderProgram.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

class ShaderProgramCompute : public ShaderProgram {
private:
	//Number of invocations per group = 100 invoc. per group
	glm::ivec3 workGroupSize{10, 10, 1}; //Fixed in comp shader
	glm::ivec3 workGroupNum{1,1,1};

public:
	void Compute()
	{
		glDispatchCompute(workGroupNum.x, workGroupNum.y, workGroupNum.z);
	}
	
	void Wait()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void SetWorkGroupSize(glm::vec3 size)
	{
		workGroupSize = size;
	}

	void SetWorkGroupNum(glm::vec3 ObjectSize)
	{
		workGroupNum={ObjectSize.x/workGroupSize.x, ObjectSize.y/workGroupSize.y, ObjectSize.z/workGroupSize.z};
	}
};
