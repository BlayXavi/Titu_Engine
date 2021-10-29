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

		void DeselectEntity() { m_LastSelectedEntity = m_EntityNull; }

	private:
		void DrawEntityNode(Entity& e);
		void DrawEntityPanel(Entity& e);

		Scene* m_Context;

		Entity m_LastSelectedEntity;
		Entity m_EntityNull;
	};
}