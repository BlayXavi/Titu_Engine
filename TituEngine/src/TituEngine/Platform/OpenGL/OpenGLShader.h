#pragma once

#include "TituEngine/Renderer/Shader.h"
#include <string>
#include <unordered_map>

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace TituEngine
{

	class OpenGLShader : public Shader
	{
	public:
		~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, const int& integer)  override;
		void SetMat4(const std::string& name, const glm::mat4& matrix) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;

		void SetIntArray(const std::string& name, const int* values, const uint32_t count) override;

		static std::string GetCacheDirectory() { return s_CacheDirectory; }

		const spirv_cross::ShaderResources* GetShaderResources() const { return &m_ShaderResources; }

		uint32_t GetTextureResourcesCount() { return (uint32_t)m_ShaderResources.sampled_images.size(); };

	private:
		friend Shader;

		OpenGLShader(const std::string& path);
		std::string ReadFile(const std::string& path);
		std::unordered_map<uint32_t, std::string> SplitStages(const std::string& source);

		bool CompileOrGetVulkanBinaries(const std::unordered_map<uint32_t, std::string>& shaderSources);
		bool CompileOrGetOpenGLBinaries();
		void CreateProgram();

		void Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData);

		int GetUniformLocation(const std::string& name);
		void ClearData();

	private:
		uint32_t m_RendererID;

		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::vector<uint32_t> glShaderIDs;

		std::string m_Path;
		std::string m_Name;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSPIRVStages;
		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRVStages;

		static std::string s_CacheDirectory;

		spirv_cross::ShaderResources m_ShaderResources;
	};
}
