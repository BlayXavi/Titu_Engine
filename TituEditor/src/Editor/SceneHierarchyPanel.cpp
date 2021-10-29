#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "TituEngine/Scene/Components.h"
#include "ComponentPanelDrawer.h"

namespace TituEngine
{
	void SceneHierarchyPanel::OnImGuiRender(Scene* const context)
	{
		m_Context = context;

		ImGui::Begin("Scene Hierarchy");

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			DeselectEntity();

		context->m_Registry.each([&](auto entityID)
			{
				Entity e{ entityID, context };
				DrawEntityNode(e);
			});

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				context->CreateEntity();
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& e)
	{
		TagComponent& tagC = e.GetComponent<TagComponent>();
		bool changed = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (m_LastSelectedEntity == e ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tagC); 
		if (ImGui::IsItemClicked(0))
		{
			m_LastSelectedEntity = e;
			changed = true;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy Entity"))
			{
				if (m_LastSelectedEntity == e)
					DeselectEntity();
				m_Context->DestroyEntity(e);
			}
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (m_LastSelectedEntity == e && !changed)
		{
			ImGui::Begin("Inspector");
			DrawEntityPanel(m_LastSelectedEntity);
			ImGui::End();
		}

	}

	void SceneHierarchyPanel::DrawEntityPanel(Entity& e)
	{
		if (e.HasComponent<TagComponent>())
		{
			TagComponent& tag = e.GetComponent<TagComponent>();
			ComponentPanelDrawer::DrawTagComponent(tag);
		}

		if (e.HasComponent<TransformComponent>())
		{
			TransformComponent& tc = e.GetComponent<TransformComponent>();
			ComponentPanelDrawer::DrawTransformComponent(tc);
		}

		if (e.HasComponent<CameraComponent>())
		{
			CameraComponent& cc = e.GetComponent<CameraComponent>();
			ComponentPanelDrawer::DrawCameraComponent(cc);
		}
	}
}