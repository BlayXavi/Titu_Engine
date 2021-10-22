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

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e = Entity(this);
		e.AddComponent<TagComponent>(name);
		return e;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//Update
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				nsc.Instance->OnUpdate(ts);
			});

		//Render
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}

