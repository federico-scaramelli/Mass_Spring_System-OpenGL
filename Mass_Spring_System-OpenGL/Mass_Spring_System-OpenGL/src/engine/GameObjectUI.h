#pragma once
#include "MaterialUI.h"
#include "TransformUI.h"

class GameObjectUI 
{
public:
	virtual ~GameObjectUI()
	{
		delete m_TransformUI;
		delete m_MaterialUI;
	}

	GameObjectUI (const char* name)
	{
		m_Name = name;
		m_TransformUI = new TransformUI(name);
	}

	virtual void Draw()
	{
		m_TransformUI->Draw();
		if (m_MaterialUI != nullptr)
		  m_MaterialUI->Draw();
	}

	const char* m_Name;
	TransformUI* m_TransformUI;
	MaterialUI* m_MaterialUI;
};
