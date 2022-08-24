#include "RendererUI.h"

#include "Camera.h"
#include "CollidingSphere.h"
#include "LightSource.h"
#include "../mass-spring/Wind.h"
#include "../mass-spring/MassSpring.h"
#include "../mass-spring/MassSpringUI.h"
#include "Scene.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void RendererUI::DrawUI ()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin ("Scene");

	// Additional settings
	if (ImGui::CollapsingHeader ("Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox ("Wireframe mode", &Renderer::GetInstance()->wireframe);
		ImGui::Checkbox ("Backface culling", &Renderer::GetInstance()->backface);
	}

	ImGui::Dummy ({ 0, 10 });
	ImGui::Separator();
	ImGui::Dummy ({ 0, 10 });

	// Camera UI
	if (ImGui::CollapsingHeader ("Camera")) { Scene::GetInstance()->GetCamera()->GetUI()->Draw(); }

	ImGui::Dummy ({ 0, 10 });
	ImGui::Separator();
	ImGui::Dummy ({ 0, 10 });

	// Light UI
	if (ImGui::CollapsingHeader ("Light")) { Scene::GetInstance()->GetLightSource()->GetUI().Draw(); }

	ImGui::Dummy ({ 0, 10 });
	ImGui::Separator();
	ImGui::Dummy ({ 0, 10 });

	// Wind UI
	if (ImGui::CollapsingHeader ("Wind")) { Scene::GetInstance()->GetWind()->GetUI().Draw(); }
	ImGui::End();

	// Primitives UI
	ImGui::Begin ("Objects");
	if (ImGui::CollapsingHeader ("Colliding spheres", ImGuiTreeNodeFlags_DefaultOpen))
	{
		AddSphereUI();

		ImGui::Dummy ({ 0, 20 });

		// GameObjects UI
		auto m = Scene::GetInstance()->GetGameObjects();
		for (auto it = m.cbegin(), next_it = it;
		     it != m.cend();
		     it = next_it)
		{
			++next_it;
			GameObject* gameObject = it->second;
			std::string label = "X##";
			label.append (gameObject->name);
			if (ImGui::Button (label.c_str(), { 20, 20 }))
			{
				Scene::GetInstance()->RemoveGameObject (it->first);
			}
			else
			{
				ImGui::SameLine();
				ImGui::Checkbox (gameObject->name.c_str(), &gameObject->m_IsActive);
				if (gameObject->m_IsActive)
				{
					ImGui::SameLine();
					if (ImGui::CollapsingHeader (gameObject->name.c_str())) { gameObject->GetUI().Draw(); }
					ImGui::Dummy ({ 0, 15 });
				}
			}
		}
	}

	ImGui::Dummy ({ 0, 20 });
	ImGui::Separator();
	ImGui::Dummy ({ 0, 20 });

	// Mass Springs UI
	if (ImGui::CollapsingHeader ("Mass Springs", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (!Scene::GetInstance()->m_MassSprings.empty())
		{
			ImGui::ListBox ("Select Mass Spring Object",
			                &MassSpringUI::selectedMassSpring,
			                MassSpringUI::massSpringsList,
			                Scene::GetInstance()->m_MassSprings.size());
			Scene::GetInstance()->m_MassSprings[MassSpringUI::selectedMassSpring]->GetUI().Draw();
		}
	}


	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}

void RendererUI::AddSphereUI ()
{
	auto colliders = Scene::GetInstance()->GetColliders();
	int i = 0;
	for (auto it = colliders.begin(); it != colliders.end();)
	{
		i++;
		++it;
	}
	if (i >= 10)
	{
		ImGui::Text ("Max number of sphere in scene reached.");
		return;
	}


	if (newSphereRadius > 150) newSphereRadius = 150;
	else if (newSphereRadius < 5) newSphereRadius = 5;
	ImGui::Text ("Create a new sphere");
	ImGui::PushItemWidth (200);
	ImGui::InputFloat ("Radius", &newSphereRadius, 2.5, 10, "%.1f");
	ImGui::SameLine();
	if (ImGui::Button ("Add Sphere", { 80, 20 }))
	{
		newSphereCount++;

		std::string name = "Sphere (";
		if (newSphereCount < 10) name += "0";
		name.append (std::to_string (newSphereCount));
		name.append (")");
		auto* newSphere = new CollidingSphere (name.c_str(), newSphereRadius);
		Scene::GetInstance()->AddGameObject (newSphere);
	}
}
