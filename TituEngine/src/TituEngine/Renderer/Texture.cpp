#include "tepch.h"
#include "Texture.h"

#include "TituEngine/Core/Core.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLTexture2D.h"

namespace TituEngine
{
	Texture2D* Texture2D::Create(uint width, uint height)
	{
		TE_PROFILE_PROFILE_FUNC();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(width, height);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(path);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}
}