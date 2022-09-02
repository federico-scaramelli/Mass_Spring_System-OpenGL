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
	// Scene frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin ("Scene");

	ImGui::PushItemWidth (200);

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



	// Sphere colliders UI
	ImGui::Begin ("Scene Objects");
	ImGui::PushItemWidth (200);
	AddSphereUI();
	ImGui::Dummy ({ 0, 20 });

	// GameObjects UI - It contains the list of created colliding spheres currently on the scene
	auto m = Scene::GetInstance()->GetGameObjects();
	for (auto it = m.cbegin(), next_it = it;
	     it != m.cend();
	     it = next_it)
	{
		++next_it;
		GameObject* gameObject = it->second;

		// Delete button
		std::string label = "X##";
		label.append (gameObject->name);
		if (ImGui::Button (label.c_str(), { 20, 20 }))
		{
			Scene::GetInstance()->RemoveGameObject (it->first);
		}
		else
		{
			ImGui::SameLine();
			// Active checkbox
			ImGui::Checkbox (gameObject->name.c_str(), &gameObject->m_IsActive);
			if (gameObject->m_IsActive)
			{
				ImGui::SameLine();
				if (ImGui::CollapsingHeader (gameObject->name.c_str())) { gameObject->GetUI().Draw(); }
				ImGui::Dummy ({ 0, 15 });
			}
		}
	}
	ImGui::End();


	// Mass Springs UI
	ImGui::Begin ("Mass Spring");
	ImGui::PushItemWidth (200);
	ImGui::Text ("Select active mass spring");
	if (!Scene::GetInstance()->GetMassSprings().empty())
	{
		// ListBox to select the active mass spring
		ImGui::ListBox ("##SelectMassSpring",
		                &MassSpringUI::selectedMassSpring,
		                MassSpringUI::massSpringsList,
		                Scene::GetInstance()->GetMassSprings().size());
		ImGui::SameLine();
		// Reset mass spring button
		if (ImGui::Button ("Reset", {80, 20}))
		{
			Scene::GetInstance()->ResetMassSpring();
		}
		std::string selectedName = MassSpringUI::massSpringsList[MassSpringUI::selectedMassSpring];
		auto massSpring = Scene::GetInstance()->GetMassSprings().at (selectedName);
		massSpring->GetUI().Draw();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}

// Present the radius selector, the creation button and check the limit of the sphere count
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
