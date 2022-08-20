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
		m_MaterialUI = new MaterialUI(name);
	}

	virtual void Draw()
	{
		m_TransformUI->Draw();
		m_MaterialUI->Draw();
	}

	const char* m_Name;
	TransformUI* m_TransformUI;
	MaterialUI* m_MaterialUI;
};
