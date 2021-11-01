#include "tepch.h"
#include "Entity.h"
#include "Scene.h"

namespace TituEngine
{
	Entity::Entity(Scene* const scene) :
		m_Scene(scene)
	{
		m_EnttHandle = scene->m_Registry.create();
	}

	Entity::Entity(const entt::entity& handle, Scene* const scene) :
		m_EnttHandle(handle), m_Scene(scene)
	{
	}
}
