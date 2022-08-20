#pragma once
#include "../engine/GameObjectUI.h"

class MassSpringUI : public GameObjectUI
{
public:
	MassSpringUI(const char* name) : GameObjectUI (name) {}

	inline static const char* sceneMassSprings[10];
	inline static int selectedMassSpring = 0;
};