#include "tepch.h"
#include "Entity.h"
#include "Scene.h"

namespace TituEngine
{
	Entity::Entity(Scene* scene)
	{
		m_Scene = scene;
		m_EnttHandle = scene->m_Registry.create();
	}
}
