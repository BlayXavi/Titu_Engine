#pragma once
#include <string>
#include "glm/glm.hpp"

#include <unordered_map>

#include "Signals.h"

namespace TituEngine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		//static Shader* Create(const std::string& vs, const std::string& ps);
		static Shader* Create(const std::string& path);
		virtual bool Recompile() = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetInt(const std::string& name, const int& value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetIntArray(const std::string& name, const int* values, const uint32_t count) = 0;

		bool CompilationSucceeded() const { return m_CompilationSucceeded; }

		virtual uint32_t GetTextureResourcesCount() = 0;

		std::string GetName() const { return m_Name; }
		std::string GetPath() const { return m_Path; }

		Signal<> ShaderRecompiled;

	protected:

		virtual bool Compile(bool useCache = true) = 0;

		bool m_CompilationSucceeded = false;

		std::string m_Path;
		std::string m_Name;

		static std::unordered_map<std::string, Shader*> m_LoadedShaders;
	};


	class ShaderUtilities
	{
	public:
		static Shader* s_ErrorShader;
		static Shader* s_DefaultShader;
		static void Init();
		static void Shutdown();
	private:
		static bool s_Initialized;
	};
}