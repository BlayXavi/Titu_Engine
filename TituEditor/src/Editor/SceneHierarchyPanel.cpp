#include "imgui/imgui.h"

#include "SceneHierarchyPanel.h"
#include "ComponentPanelDrawer.h"
#include "TituEngine/Scene/Components.h"
#include "TituEngine/Core/Log.h"

#include "../TituEditor.h"

namespace TituEngine
{
	bool SceneHierarchyPanel::m_OpenSceneHierarchy = true;

	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_DeselectEntityHandler = TituEditorLayer::OnSceneLoaded.AddListener(this, &SceneHierarchyPanel::DeselectEntity);
	}

	SceneHierarchyPanel::~SceneHierarchyPanel()
	{
		TituEditorLayer::OnSceneLoaded.RemoveListener(m_DeselectEntityHandler);
	}

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

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
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
			else if (ImGui::MenuItem("Duplicate Entity"))
			{
				Entity copyE = m_Context->CreateCopy(e);
				m_SelectedEntity = copyE;
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
		bool opened = true;
		ImGui::Begin("Inspector", &opened);
		if (!opened)
		{
			DeselectEntity();
		}
		else
		{
			if (e.HasComponent<TagComponent>())
			{
				TagComponent& tag = e.GetComponent<TagComponent>();
				ComponentPanelDrawer::DrawTagComponent(e, tag);
			}

			ImGui::SameLine();
			if (ImGui::Button("Add Component"))
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
				else if (ImGui::MenuItem("SpriteRenderer Component"))
				{
					m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::MenuItem("ModelRenderer Component"))
				{
					m_SelectedEntity.AddComponent<ModelRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				else if (ImGui::MenuItem("Light Component"))
				{
					m_SelectedEntity.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
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

			if (e.HasComponent<ModelRendererComponent>())
			{
				ModelRendererComponent& mrC = e.GetComponent<ModelRendererComponent>();
				ComponentPanelDrawer::DrawComponent(e, mrC);
			}

			if (e.HasComponent<LightComponent>())
			{
				LightComponent& lc = e.GetComponent<LightComponent>();
				ComponentPanelDrawer::DrawComponent(e, lc);
			}
		}

		ImGui::End();
	}
}