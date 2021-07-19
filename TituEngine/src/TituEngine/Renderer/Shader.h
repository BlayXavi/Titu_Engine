#pragma once
#include <string>
#include "glm/glm.hpp"

#include <unordered_map>

namespace TituEngine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformInt(const std::string& name, const int& integer);

	private:
		uint m_Program, m_VertexShader, m_FragmentShader;
		std::unordered_map<std::string, int> m_UniformLocationCache;

		int GetUniformLocation(const std::string& name);
	};
}