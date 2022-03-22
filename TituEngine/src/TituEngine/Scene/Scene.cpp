#include "tepch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "TituEngine/Renderer/Renderer2D.h"
#include "TituEngine/Renderer/Renderer3D.h"
#include "TituEngine/Renderer/Framebuffer.h"

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

	Entity Scene::CreateCopy(Entity blueprint)
	{
		Entity copy = CreateEntity(blueprint.GetComponent<TagComponent>());

		if (blueprint.HasComponent<TransformComponent>())
			copy.AddComponent<TransformComponent>(blueprint.GetComponent<TransformComponent>());
		if (blueprint.HasComponent<CameraComponent>())
			copy.AddComponent<CameraComponent>(blueprint.GetComponent<CameraComponent>());
		if (blueprint.HasComponent<SpriteRendererComponent>())
			copy.AddComponent<SpriteRendererComponent>(blueprint.GetComponent<SpriteRendererComponent>());
		if (blueprint.HasComponent<ModelRendererComponent>())
			copy.AddComponent<ModelRendererComponent>(blueprint.GetComponent<ModelRendererComponent>());
		if (blueprint.HasComponent<LightComponent>())
			copy.AddComponent<LightComponent>(blueprint.GetComponent<LightComponent>());

		return copy;
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

		Renderer::UploadCameraDataToGPU();

		Renderer2D::BeginScene();
		{
			TE_PROFILE_PROFILE_SCOPE("Render2D::BeginDraw");
			auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite, (int32_t)entity);
			}
		}
		Renderer2D::EndScene();

		Renderer3D::BeginScene();
		{
			TE_PROFILE_PROFILE_SCOPE("Render3D::BeginDraw");


			auto groupL = m_Registry.view<TransformComponent, LightComponent>();
			for (auto entity : groupL)
			{
				auto& [transform, light] = groupL.get<TransformComponent, LightComponent>(entity);
				Renderer3D::UploadLight(light, transform);
			}

			Renderer3D::UploadLightDataToGPU();

			DeferredShadingPass();

			////Render
			//auto group = m_Registry.view<TransformComponent, ModelRendererComponent>();
			//for (auto entity : group)
			//{
			//	auto& [transform, modelRenderer] = group.get<TransformComponent, ModelRendererComponent>(entity);

			//	Renderer3D::DrawModel(transform, modelRenderer);
			//}
		}
		Renderer3D::EndScene();
	}

	void Scene::DeferredGBufferPass()
	{

		Renderer::UploadCameraDataToGPU();

		auto group = m_Registry.view<TransformComponent, ModelRendererComponent>();
		for (auto entity : group)
		{
			auto& [transform, modelRenderer] = group.get<TransformComponent, ModelRendererComponent>(entity);

			Renderer3D::DrawModel(transform, modelRenderer, (int32_t)entity, ShaderUtilities::s_GBufferShader);
		}
	}

	void Scene::DeferredShadingPass()
	{
		Framebuffer* fb = Renderer::GetGBuffer();
		fb->BindColorAttachments();

		Renderer::GetQuad()->Render(ShaderUtilities::s_GBufferShadingPass);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Registry.view<CameraComponent>().each([=](auto entity, CameraComponent& cc)
			{
				cc.Camera.SetViewportSize(width, height);
			});
	}
}
