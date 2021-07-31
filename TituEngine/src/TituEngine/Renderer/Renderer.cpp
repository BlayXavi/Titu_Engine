#include "tepch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

namespace TituEngine
{
	RendererAPI::API RendererAPI::s_RendererAPI = API::OpenGL;

	void Renderer::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		Renderer2D::Init();
		RenderCommand::Init();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
