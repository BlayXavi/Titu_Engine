#include "tepch.h"

#include "Texture.h"
#include "Shader.h"

#include "Renderer.h"
#include "Renderer2D.h"

#include "TituEngine/Platform/OpenGL/OpenGlRendererAPI.h"

namespace TituEngine
{
	//---------------------------------RENDERER API---------------------------------
	RendererAPI::API RendererAPI::s_RendererAPIID = API::OpenGL;
	
	RendererAPI* RendererAPI::Create()
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLRendererAPI();
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Renderer API]"); return nullptr;
	}

	//---------------------------------  RENDERER  ---------------------------------
	void Renderer::Init()
	{
		TE_PROFILE_PROFILE_FUNC();

		TextureUtilities::Init();
		ShaderUtilities::Init();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		RenderCommand::Shutdown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	//-----------------------------------RENDER COMMAND------------------
	RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();

	
}
