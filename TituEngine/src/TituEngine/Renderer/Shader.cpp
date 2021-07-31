#include "tepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "TituEngine/Platform/OpenGL/OpenGLShader.h"

namespace TituEngine
{
	Shader* Shader::Create(const std::string& vs, const std::string& ps)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLShader(vs, ps);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}

	Shader* Shader::Create(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLShader(path);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}
}
