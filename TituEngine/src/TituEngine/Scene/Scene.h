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
		void OnUpdate(Timestep ts);

	private:
		friend Entity;
		entt::registry m_Registry;	
	};
}