#pragma once

#include "TituEngine/Renderer/Shader.h"
#include <string>

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

		uint m_Program, m_VertexShader, m_FragmentShader;
		std::unordered_map<std::string, int> m_UniformLocationCache;

		int GetUniformLocation(const std::string& name);

		friend Shader;
	};
}