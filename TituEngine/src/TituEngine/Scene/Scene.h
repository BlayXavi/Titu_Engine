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
		void OnUpdate(Timestep ts);

		entt::registry m_Registry;	
	};
}