#pragma once

#include "TituEngine/Scene/Scene.h"
#include "TituEngine/Scene/Entity.h"

namespace TituEngine
{
	class SceneHierarchyPanel
	{
	public:
		static bool m_OpenSceneHierarchy;

		SceneHierarchyPanel();
		virtual ~SceneHierarchyPanel();

		void OnImGuiRender(Scene* const m_Context);

		Entity GetSelectedEntity() const { return m_SelectedEntity; }

	private:
		int m_DeselectEntityHandler = -1;
		void DeselectEntity() { m_SelectedEntity = m_EntityNull; }

		void DrawEntityNode(Entity& e);
		void DrawEntityPanel(Entity& e);

		Scene* m_Context = nullptr;

		Entity m_SelectedEntity;
		Entity m_EntityNull;
	};
}