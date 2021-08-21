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

		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetInt(const std::string& name, const int& value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetIntArray(const std::string& name, const int* values, const uint32_t count) = 0;
	};
}