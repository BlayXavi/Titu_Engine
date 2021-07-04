#pragma once
#include <string>
#include "glm/glm.hpp"
namespace TituEngine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		uint m_Program, m_VertexShader, m_FragmentShader;
	};
}