#include "tepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "TituEngine/Platform/OpenGL/OpenGLShader.h"

namespace TituEngine
{

	Shader* Shader::Create(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		Shader* shader = nullptr;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			shader = new OpenGLShader(path);
			break;
		default:
			TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]");
			break;
		}

		if (shader != nullptr)
		{
			if (shader->CompilationSucceeded())
				return shader;
			else
				delete shader;
		}

		return ShaderUtilities::s_ShaderError;;
	}


	Shader* ShaderUtilities::s_ShaderError = nullptr;
	bool ShaderUtilities::s_Initialized = false;

	void ShaderUtilities::Init()
	{
		s_Initialized = true;
		s_ShaderError = Shader::Create("assets/shaders/testing/ErrorShader.glsl");
	}

	void ShaderUtilities::Shutdown()
	{
		if (!s_Initialized)
			return;
		delete s_ShaderError;
	}
}
