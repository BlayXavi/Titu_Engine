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

		template<typename T, typename ...Args>
		T& AddOrGetComponent(Args&&... args)
		{
			if (!HasComponent<T>())
				return AddComponent<T>(std::forward<Args>(args)...);

			T& c = GetComponent<T>();
			c = T(std::forward<Args>(args)...);
			return  c;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registry.emplace<T>(m_EnttHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EnttHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			T* c = m_Scene->m_Registry.try_get<T>(m_EnttHandle);
			if (c == nullptr)
				return false;
			return true;
		}

	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EnttHandle = entt::null;
	};
}