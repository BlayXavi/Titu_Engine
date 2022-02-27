#include "tepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "TituEngine/Platform/OpenGL/OpenGLShader.h"

namespace TituEngine
{
	std::unordered_map<std::string, Shader*> Shader::m_LoadedShaders;

	Shader* Shader::Create(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		Shader* shader = nullptr;

		auto loadedShader = m_LoadedShaders.find(path);
		if (loadedShader != m_LoadedShaders.end())
			return loadedShader->second;

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
			if (shader->CompilationSucceeded() == false)
			{
				delete shader;
				shader = ShaderUtilities::s_ErrorShader;
			}
		}
		else
			shader = ShaderUtilities::s_ErrorShader;

		m_LoadedShaders.insert(std::make_pair(path, shader));
		return shader;
	}


	Shader* ShaderUtilities::s_ErrorShader = nullptr;
	Shader* ShaderUtilities::s_DefaultShader = nullptr;

	bool ShaderUtilities::s_Initialized = false;

	void ShaderUtilities::Init()
	{
		if (s_Initialized)
			return;

		s_Initialized = true;
		s_ErrorShader = Shader::Create("assets\\shaders\\testing\\ErrorShader.glsl");
		s_DefaultShader = Shader::Create("assets\\shaders\\testing\\DefaultShader.glsl");
	}

	void ShaderUtilities::Shutdown()
	{
		if (!s_Initialized)
			return;

		delete s_ErrorShader;
	}
}
