#pragma once
#include <vector>
#include "MassSpring.h"

class Wind;
class CollidingSphere;

#define TARGET_FPS 30.0
#define FIXED_DELTAT (1.0 / TARGET_FPS)

class PhysicsSolver
{
public:
	PhysicsSolver() = default;

	inline static double deltaTime = 0;
	const double fixedDeltaTime = FIXED_DELTAT;
	double lastTime = 0;
	double nowTime = 0;

	void SetActiveMassSpring (MassSpring* massSpring) { this->massSpring = massSpring; }
	void SetActiveWind (Wind* wind) { this->wind = wind; }

	void AddCollider (CollidingSphere* collider) { colliders.push_back (collider); }

	void Update()
	{
		if ( massSpring == nullptr ) return;

		if(!colliders.empty())
			UpdateCollidingSphereUniforms ();

		if(wind != nullptr)
			UpdateWindUniforms ();
	}

	std::vector<CollidingSphere*> colliders;

private:
	MassSpring* massSpring = nullptr;

	Wind* wind = nullptr;

	void UpdateCollidingSphereUniforms ();

	void UpdateWindUniforms ();
};
