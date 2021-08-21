#include "tepch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace TituEngine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const VertexArray* vertexArray, const uint count)
	{
		glDrawElements(GL_TRIANGLES, count == -1 ? vertexArray->GetIndexBuffer()->GetCount() : count, GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}
