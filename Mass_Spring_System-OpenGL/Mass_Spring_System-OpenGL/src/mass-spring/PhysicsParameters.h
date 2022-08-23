#pragma once

#define TARGET_FPS 60.0
#define FIXED_DELTAT (1.0 / TARGET_FPS)

class Physics
{
public:
	inline static double deltaTime = 0;
	inline static const double fixedDeltaTime = FIXED_DELTAT;
	inline static double lastTime = 0;
	inline static double nowTime = 0;	
};
