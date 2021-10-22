#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "TituEngine/Scene/Components.h"
#include "ComponentPanelDrawer.h"

namespace TituEngine
{
	void SceneHierarchyPanel::OnImGuiRender(Scene* const context)
	{
		ImGui::Begin("Scene Hierarchy");
		context->m_Registry.each([&](auto entityID)
			{
				Entity e{ entityID, context };
				DrawEntityNode(e);
			});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& e)
	{
		TagComponent& tagC = e.GetComponent<TagComponent>();

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, ImGuiTreeNodeFlags_OpenOnArrow, tagC);
		if (ImGui::IsItemClicked)
			m_LastSelectedEntity = e;

		if (opened)
		{
			DrawEntityPanel(m_LastSelectedEntity);
			ImGui::TreePop();
		}
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