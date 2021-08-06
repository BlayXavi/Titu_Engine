#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

#include "Shader.h"

namespace TituEngine
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() const = 0;

		virtual void DrawIndexed(const VertexArray* vertexArray) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		inline static API GetAPIID() { return s_RendererAPI; }
		static RendererAPI* Create();

	private:
		static API s_RendererAPI;
	};

	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();
		static void OnWindowResized(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPIID(); }
	};

	class RenderCommand
	{
	public:

		inline static void Init() { s_RendererAPI->Init(); }
		inline static void Shutdown() { delete s_RendererAPI; }
		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		void static DrawIndexed(const VertexArray* vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

		void static SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};


}
