#pragma once

#include "TituEngine/Scene/Scene.h"
#include "TituEngine/Scene/Entity.h"

namespace TituEngine
{
	class SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel() = default;
		virtual ~SceneHierarchyPanel() = default;

		void OnImGuiRender(Scene* const m_Context);

	private:
		void DrawEntityNode(Entity& e);
		void DrawEntityPanel(Entity& e);
		Entity m_LastSelectedEntity;
	};
}