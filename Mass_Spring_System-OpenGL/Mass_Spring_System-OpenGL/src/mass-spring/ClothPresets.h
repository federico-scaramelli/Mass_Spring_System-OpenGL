#pragma once
#include <unordered_map>

#include "MassSpring.h"
#include "Cloth.h"

class ClothPreset
{
public:
	ClothPreset (MassSpringParameters parameters,
	             uint16_t pointsByWidth,
	             uint16_t pointsByHeight,
	             float restLenghtHV,
				glm::vec3 startingPos,
				glm::vec3 startingRot) :
		parameters (parameters),
		pointsByWidth (pointsByWidth),
		pointsByHeight (pointsByHeight),
		restLenghtHV (restLenghtHV),
		startingPos (startingPos),
		startingRot (startingRot){}

	MassSpringParameters parameters;
	uint16_t pointsByWidth;
	uint16_t pointsByHeight;
	float restLenghtHV;
	glm::vec3 startingPos;
	glm::vec3 startingRot;
};

class ClothPresets
{
public:

	inline static ClothPreset curtain
	{
		{0.016f, 16, 0.95f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 10.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 0.8f, 0.33f, 0.95f},
		50, 50,
		5,
		{0, 0, 0},
		{0, 0, 0}
	};

	inline static ClothPreset flag
	{
	  {0.016f, 16, 0.95f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 10.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 0.8f, 0.33f, 0.95f},
		50, 37,
		6.0f,
		{50, 0, 0},
		{0, 0, 0}
	};

	inline static ClothPreset trampoline
	{
	  {0.016f, 16, 0.95f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 10.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 0.8f, 0.33f, 0.95f},
		50, 50,
		5.0f,
		{0, -200, 0},
		{90, 0, 0}
	};

	inline static ClothPreset towel
	{
	  {0.016f, 16, 0.95f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 10.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 0.8f, 0.33f, 0.95f},
		50, 50,
		5.0f,
		{0, 0, 0},
		{0, 0, 0}
	};
};
