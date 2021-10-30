#include "imgui/imgui.h"

#include "SceneHierarchyPanel.h"
#include "ComponentPanelDrawer.h"
#include "TituEngine/Scene/Components.h"
#include "TituEngine/Core/Log.h"
namespace TituEngine
{
	bool SceneHierarchyPanel::m_OpenSceneHierarchy = true;

	void SceneHierarchyPanel::OnImGuiRender(Scene* const context)
	{
		m_Context = context;
		
		if (m_OpenSceneHierarchy)
		{
			ImGui::Begin("Scene Hierarchy", &m_OpenSceneHierarchy);

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
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity& e)
	{
		TagComponent& tagC = e.GetComponent<TagComponent>();
		bool changed = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (m_SelectedEntity == e ? ImGuiTreeNodeFlags_Selected : 0);
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tagC);
		if (ImGui::IsItemClicked(0))
		{
			m_SelectedEntity = e;
			changed = true;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy Entity"))
			{
				if (m_SelectedEntity == e)
					DeselectEntity();
				m_Context->DestroyEntity(e);
			}
			ImGui::EndPopup();
		}

		if (opened)
			ImGui::TreePop();

		if (m_SelectedEntity == e && !changed)
		{
			DrawEntityPanel(m_SelectedEntity);
		}

	}

	void SceneHierarchyPanel::DrawEntityPanel(Entity& e)
	{
		ImGui::Begin("Inspector");

		if (e.HasComponent<TagComponent>())
		{
			TagComponent& tag = e.GetComponent<TagComponent>();
			ComponentPanelDrawer::DrawComponent(e, tag);
		}

		if (e.HasComponent<TransformComponent>())
		{
			TransformComponent& tc = e.GetComponent<TransformComponent>();
			ComponentPanelDrawer::DrawComponent<TransformComponent>(e, tc);
		}

		if (e.HasComponent<CameraComponent>())
		{
			CameraComponent& cc = e.GetComponent<CameraComponent>();
			ComponentPanelDrawer::DrawComponent(e, cc);
		}

		if (e.HasComponent<SpriteRendererComponent>())
		{
			SpriteRendererComponent& srC = e.GetComponent<SpriteRendererComponent>();
			ComponentPanelDrawer::DrawComponent(e, srC);
		}

		if (ImGui::Button("Add Componnet"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Transform Component"))
			{
				m_SelectedEntity.AddComponent<TransformComponent>();
				ImGui::CloseCurrentPopup();
			}
			else if (ImGui::MenuItem("Camera Component"))
			{
				m_SelectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("SpriteRenderer Component"))
			{
				m_SelectedEntity.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}