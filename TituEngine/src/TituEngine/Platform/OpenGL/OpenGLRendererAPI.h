#pragma once

#include "TituEngine/Renderer/Renderer.h"
namespace TituEngine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		OpenGLRendererAPI() {}

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() const override;

		virtual void DrawIndexed(const VertexArray* vertexArray) override;

	};
}