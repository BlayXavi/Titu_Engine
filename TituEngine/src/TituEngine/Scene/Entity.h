#pragma once

#include "TituEngine/Core/Core.h"
#include "entt.hpp"

namespace TituEngine
{
	class Scene;

	class Entity
	{
	public:
		Entity(Scene* scene);
		~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_EnttHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return scene->m_Registry.get<T>(m_EnttHandle);
		}

		template<typename T>
		bool HasComponent()
		{	
			m_Scene->m_Registry.has<T>(m_EnttHandle);
		}

	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EnttHandle = entt::null;
	};
}