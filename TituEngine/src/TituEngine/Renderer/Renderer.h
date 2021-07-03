#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

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

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() const = 0;

		virtual void DrawIndexed(const VertexArray* vertexArray) = 0;

		inline static API GetAPIID() { return s_RendererAPI; }

	private:
		static API s_RendererAPI;
	};

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const VertexArray* vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPIID(); }
	};

	class RenderCommand
	{
	public:

		inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }

		void static DrawIndexed(const VertexArray* vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};


}
