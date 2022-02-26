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

		return ShaderUtilities::s_ErrorShader;;
	}


	Shader* ShaderUtilities::s_ErrorShader = nullptr;
	Shader* ShaderUtilities::s_DefaultShader = nullptr;

	bool ShaderUtilities::s_Initialized = false;

	void ShaderUtilities::Init()
	{
		if (s_Initialized)
			return;

		s_Initialized = true;
		s_ErrorShader = Shader::Create("assets/shaders/testing/ErrorShader.glsl");
		s_DefaultShader = Shader::Create("assets/shaders/testing/DefaultShader.glsl");
	}

	void ShaderUtilities::Shutdown()
	{
		if (!s_Initialized)
			return;

		delete s_ErrorShader;
	}
}
