#include "tepch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "TituEngine/Renderer/Renderer2D.h"

namespace TituEngine
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		m_Registry.clear();
	}

	Entity Scene::CreateEntity()
	{
		return Entity(this);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}

