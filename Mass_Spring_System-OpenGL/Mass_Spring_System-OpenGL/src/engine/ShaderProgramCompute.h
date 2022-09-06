#pragma once

#include "ShaderProgram.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

// Shader program specific for compute shaders
class ShaderProgramCompute : public ShaderProgram {
private:
	glm::ivec3 workGroupSize{16, 16, 1};
	glm::ivec3 workGroupNum{1,1,1};

public:

	void CreateProgram (std::pair<std::string, ShaderType> pairList)
	{
		CompileShader(pairList.first, pairList.second);
		Link();
		Validate();
	}

	void Compute()
	{
		glDispatchCompute(workGroupNum.x, workGroupNum.y, workGroupNum.z);
	}
	
	void Wait()
	{
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}

	void SetWorkGroupSize(glm::vec3 size)
	{
		workGroupSize = size;
	}

	void SetWorkGroupNum(glm::vec3 ObjectSize)
	{
		
		workGroupNum={
			(ObjectSize.x + workGroupSize.x -1)/workGroupSize.x, 
			(ObjectSize.y + workGroupSize.y -1)/workGroupSize.y, 
			ObjectSize.z/workGroupSize.z
		};
	}
};
