#include "RendererUI.h"

#include "Camera.h"
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
	if (ImGui::CollapsingHeader ("Primitives", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (newSphereRadius > 150) newSphereRadius = 150;
		else if (newSphereRadius < 5) newSphereRadius = 5;
		ImGui::InputFloat ("Radius: ", &newSphereRadius, 2.5, 5, "%.1f");
		if (ImGui::Button ("Add Sphere", { 100, 25 }))
		{
			std::cout << newSphereRadius << std::endl;
		}

		// GameObjects UI
		for (int i = 0; i < Scene::GetInstance()->GetGameObjectCount(); i++)
		{
			GameObject* gameObject = Scene::GetInstance()->GetGameObjects()[i];

			ImGui::Checkbox (gameObject->GetUI().m_Name, &gameObject->m_IsActive);

			if (!gameObject->m_IsActive) continue;

			if (ImGui::CollapsingHeader (gameObject->GetUI().m_Name)) { gameObject->GetUI().Draw(); }
			ImGui::Dummy ({ 0, 15 });
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
