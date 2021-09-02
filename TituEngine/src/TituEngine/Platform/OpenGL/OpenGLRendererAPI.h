#pragma once

#include "TituEngine/Renderer/Renderer.h"
namespace TituEngine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() = default;

		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() const override;

		virtual void DrawIndexed(const VertexArray* vertexArray, const uint count = -1) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void EnableDepthTest(bool enable) override;

	private:
		friend RendererAPI;
	};
}