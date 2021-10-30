#pragma once
#include "TituEngine/Scene/Components.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <memory>
namespace TituEngine
{
	class ComponentPanelDrawer
	{
	public:

		static void DrawTagComponent(Entity& e, TagComponent& tag);

		template<typename C>
		static void DrawComponent(Entity& e, C& Component)
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(C).hash_code(), m_TreeNodeFlags, Component.PrettyName);
			bool removed = false;
			ComponentSettings<C>(e, removed);
			if (open)
			{
				if (!removed)
					DrawComponentInternal(e, Component);
				ImGui::TreePop();
			}
		}

		static void DrawCamera(Camera& cam);
		//static void DrawTransform(glm::mat4& transform);
		static bool DrawVec3(std::string label, glm::vec3& values, const glm::vec3& resetValue);

	private:
		static ImGuiTreeNodeFlags m_TreeNodeFlags;

		template<typename C>
		static void DrawComponentInternal(Entity& e, C& Component);

		template<class T>
		static void ComponentSettings(Entity& e, bool& removed)
		{
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Destroy Component"))
				{
					e.DestroyComponent<T>();
					removed = true;
				}

				ImGui::EndPopup();
			}
		}
	};
}