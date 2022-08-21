#include "MassSpring.h"

MassSpring::MassSpring(const char* name, MassSpringParameters parameters) :
	  GameObject (name),
	  m_Parameters (std::move (parameters))
	{
		m_GameObjectUI = new MassSpringUI(name);
	}
