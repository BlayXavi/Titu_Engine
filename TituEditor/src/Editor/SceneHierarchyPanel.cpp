#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "TituEngine/Scene/Components.h"
#include "ComponentPanelDrawer.h"

namespace TituEngine
{
	void SceneHierarchyPanel::OnImGuiRender(Scene* const context)
	{
		context->m_Registry.each([&](auto entityID)
			{
				Entity e{ entityID, context };
				DrawEntityNode(e);
			});

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& e)
	{
		ImGui::Begin("Scene Hierarchy");

		TagComponent& tagC = e.GetComponent<TagComponent>();

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, ImGuiTreeNodeFlags_OpenOnArrow, tagC);
		if (ImGui::IsItemClicked(0))
			m_LastSelectedEntity = e;

		if (opened)
			ImGui::TreePop();
		ImGui::End();

		ImGui::Begin("Inspector");
		if (m_LastSelectedEntity == e)
			DrawEntityPanel(m_LastSelectedEntity);
		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityPanel(Entity& e)
	{
		if (e.HasComponent<CameraComponent>())
		{
			CameraComponent& cc = e.GetComponent<CameraComponent>();
			ComponentPanelDrawer::DrawCameraComponent(cc);
		}
	}
}