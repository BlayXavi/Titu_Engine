#pragma once

#include "TituEngine/Renderer/Shader.h"
#include <string>
#include <unordered_map>

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

	private:
		friend Shader;

		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		void Compile(std::unordered_map<uint, std::string> shaderSources);

		uint m_RendererID;

		std::unordered_map<std::string, int> m_UniformLocationCache;
		int GetUniformLocation(const std::string& name);

		std::vector<uint>glShaderIDs;

		void ClearData();

		std::string m_Path;

	};
}
