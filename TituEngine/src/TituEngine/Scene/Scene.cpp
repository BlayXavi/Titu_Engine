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
		Entity e = Entity(this);
		e.AddComponent<TagComponent>("Empty_Entity");
		return e;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e = Entity(this);
		e.AddComponent<TagComponent>(name);
		return e;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{
		//Update
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Registry.view<CameraComponent>().each([=](auto entity, CameraComponent& cc)
			{
				cc.Camera.SetViewportSize(width, height);
			});
	}
}
