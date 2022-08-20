#pragma once
#include <vector>
#include "MassSpring.h"

class CollidingSphere;

#define FIXED_DELTAT (1.0 / 120.0)

class PhysicsSolver
{
public:
	PhysicsSolver() = default;

	inline static double deltaTime = 0;
	const double fixedDeltaTime = FIXED_DELTAT;
	double lastTime = 0;
	double nowTime = 0;

	void SetActiveMassSpring (MassSpring* massSpring) { this->massSpring = massSpring; }
	void AddCollider (CollidingSphere* collider) { colliders.push_back (collider); }

	void Update()
	{
		if ( massSpring == nullptr || colliders.empty() ) return;

		UpdateCollidingSphereUniforms ();
	}

	std::vector<CollidingSphere*> colliders;

private:
	MassSpring* massSpring = nullptr;

	void UpdateCollidingSphereUniforms ();
};
