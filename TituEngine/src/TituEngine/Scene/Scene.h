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

		Entity CreateCopy(Entity blueprint);

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void RenderScene();

		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry m_Registry;
	private:
		void DeferredGBufferPass();
		void DeferredShadingPass();
		void ForwardRenderingPass();

	private:

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}