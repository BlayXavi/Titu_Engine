#pragma once
#include "entt.hpp"
#include "TituEngine/Core/Timestep.h"

namespace TituEngine
{
	class Entity;

	class Scene
	{
	public: 
		Scene();
		virtual ~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;	
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}