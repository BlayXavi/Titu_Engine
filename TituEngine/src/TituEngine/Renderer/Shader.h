#pragma once
#include <string>

namespace TituEngine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint m_Program, m_VertexShader, m_FragmentShader;
	};
}