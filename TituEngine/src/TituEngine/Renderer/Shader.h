#pragma once
#include <string>
#include "glm/glm.hpp"

#include <unordered_map>

namespace TituEngine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& vs, const std::string& ps);
		static Shader* Create(const std::string& path);
	};
}