#pragma once
#include "MaterialUI.h"
#include "TransformUI.h"

// Default GameObject UI with Transform and Material UIs
class GameObjectUI 
{
public:
	virtual ~GameObjectUI()
	{
		delete m_TransformUI;
		delete m_MaterialUI;
	}

	GameObjectUI (const std::string& name)
	{
		m_Name = name.c_str();
		m_TransformUI = new TransformUI(name);
	}

	virtual void Draw()
	{
		ImGui::Dummy ({ 0, 10 });
		m_TransformUI->Draw();

		if (m_MaterialUI != nullptr)
		{
			ImGui::Dummy ({ 0, 10 });
			m_MaterialUI->Draw();
		}
	}

	const char* m_Name;
	TransformUI* m_TransformUI;
	MaterialUI* m_MaterialUI;
};
