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

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformInt(const std::string& name, const int& integer);

	private:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& path);
		void Compile(std::unordered_map<uint, std::string> shaderSources);

		uint m_RendererID;

		std::unordered_map<std::string, int> m_UniformLocationCache;
		int GetUniformLocation(const std::string& name);

		std::vector<uint>glShaderIDs;

		void ClearData();

		friend Shader;
	};
}
