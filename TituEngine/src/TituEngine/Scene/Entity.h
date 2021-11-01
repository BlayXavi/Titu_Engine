#pragma once

#include "TituEngine/Core/Core.h"
#include "TituEngine/Scene/Scene.h"

#include "entt.hpp"

namespace TituEngine
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(Scene* const scene);
		Entity(const Entity& other) = default;
		Entity(const entt::entity& eHandle, Scene* const scene);

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
			if (HasComponent<T>())
			{
				TE_ASSERT(false, "COMPONENT ALREADY ADDED");
				return GetComponent<T>();
			}

			return m_Scene->m_Registry.emplace<T>(m_EnttHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EnttHandle);
		}

		template<typename T>
		T& GetComponent() const
		{
			return m_Scene->m_Registry.get<T>(m_EnttHandle);
		}

		template<typename T>
		bool HasComponent() const
		{
			T* c = m_Scene->m_Registry.try_get<T>(m_EnttHandle);
			if (c == nullptr)
				return false;
			return true;
		}

		template<typename T>
		void DestroyComponent()
		{
			if (!HasComponent<T>())
			{
				TE_ASSERT(false, "ENTITY DOES NOT HAVE COMPONENT");
				return;
			}
			m_Scene->m_Registry.remove<T>(m_EnttHandle);
		}

		operator bool() const { return m_EnttHandle != entt::null; }
		bool operator ==(Entity other) const { return m_EnttHandle == other.m_EnttHandle && m_Scene == other.m_Scene; }
		bool operator !=(Entity other) const { return !(other == *this); }
		operator entt::entity() const { return m_EnttHandle; }
		operator uint32_t() const { return (uint32_t)m_EnttHandle; }

	private:
		Scene* m_Scene = nullptr;
		entt::entity m_EnttHandle{ entt::null };
	};

}