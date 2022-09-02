#pragma once
#include <unordered_map>

#include "MassSpring.h"
#include "Cloth.h"

// Class containing all the cloths presets
class ClothPreset
{
public:
	ClothPreset (MassSpringParameters parameters,
	             uint16_t pointsByWidth,
	             uint16_t pointsByHeight,
	             float restLenghtHV,
				glm::vec3 startingPos,
				glm::vec3 startingRot,
				glm::vec3 diffuseData, 
				glm::vec3 specularData, 
				glm::vec3 ambientData, 
				float shininessData) :

		parameters (parameters),
		pointsByWidth (pointsByWidth),
		pointsByHeight (pointsByHeight),
		restLenghtHV (restLenghtHV),
		startingPos (startingPos),
		startingRot (startingRot),
		diffuseMat (diffuseData),
		specularMat (specularData),
		ambientMat (ambientData),
		shininessMat (shininessData)
		
	{}

	MassSpringParameters parameters;
	uint16_t pointsByWidth;
	uint16_t pointsByHeight;
	float restLenghtHV;

	glm::vec3 startingPos;
	glm::vec3 startingRot;

	glm::vec3 diffuseMat;
	glm::vec3 specularMat; 
	glm::vec3 ambientMat;
	float shininessMat;
};

class ClothPresets
{
public:

	inline static ClothPreset curtain
	{
		{0.016f, 16, 0.85f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 500.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 1.0f, 0.33f, 0.7f},
		50, 50,
		5,
		{0, 0, 0},
		{0, 180, 0},
		{0.15, 0.51, 0.74},
		{0.15, 0.25, 0.59},
		{0, 0, 0},
		50
	};

	inline static ClothPreset flag
	{
	  {0.016f, 16, 1.05f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 500.0f, 1.0f, 1.0f,
		0.25f, 1.05f, 1.0f, 0.33f, 0.825f},
		50, 37,
		6.0f,
		{200, 0, 0},
		{0, 0, 0},
		{0.68f, 0.23f, 0.23f},
		{0.08, 0.07, 0.07},
		{0, 0, 0},
		50
	};

	inline static ClothPreset trampoline
	{
	  {0.016f, 16, 5.5f,
		{ 0.f, -60, 0.f, 0.f },
		1.0f, 100000.0f, 1.0f, 2.0f,
		0.25f, 1, 1.0f, 1.0f, 0},
		50, 50,
		5.0f,
		{0, -200, 0},
		{90, 0, 0},
		{0.15, 0.47, 1.0},
		{0, 0, 0},
		{0, 0, 0},
		50
	};

	inline static ClothPreset towel
	{
	  {0.016f, 16, 1.3f,
		{ 0.f, -1500, 0.f, 0.f },
		1.0f, 500.0f, 1.0f, 1.0f,
		0.25f, 1.1f, 1.0f, 0.33f, 0},
		50, 50,
		5.0f,
		{0, 0, 0},
		{-90, 0, 0},
		{0.49, 0.36, 0.66},
		{0, 0, 0},
		{0, 0, 0},
		50
	};
};
