#include "tepch.h"
#include "OpenGLShader.h"
#include "TituEngine/Core/Core.h"

#include <fstream>
#include <glad/glad.h>

namespace TituEngine
{
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		m_Path = path;
		std::ifstream in(path, std::ios::in, std::ios::binary);
		if (in)
		{
			std::string shaderSource;
			in.seekg(0, std::ios::end);
			shaderSource.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&shaderSource[0], shaderSource.size());
			in.close();

			const char* typeToken = "#type";

			size_t typeTokenLength = strlen(typeToken);
			size_t pos = shaderSource.find(typeToken, 0);

			std::unordered_map<GLenum, std::string> shaderSources;
			const char* eolChar = "\r\n";

			while (pos != std::string::npos)
			{
				size_t eol = shaderSource.find_first_of(eolChar, pos);
				if (eol == std::string::npos)
				{
					TE_CORE_ERROR("Shader Syntax Error {0}", path);
					return;
				}
				size_t begin = pos + typeTokenLength + 1;
				std::string type = shaderSource.substr(begin, eol - begin);
				GLenum shaderType = GL_NONE;
				if (type == "vertex")
					shaderType = GL_VERTEX_SHADER;
				else if (type == "fragment" || type == "pixel")
					shaderType = GL_FRAGMENT_SHADER;
				else
				{
					TE_CORE_ERROR("UNSUPORTED SHADER TYPE [shaderType] on  {0}", path);
					return;
				}

				size_t nexLinePos = shaderSource.find_first_of(eolChar, eol);
				pos = shaderSource.find(typeToken, nexLinePos);
				shaderSources[shaderType] = shaderSource.substr(nexLinePos, pos - (nexLinePos == std::string::npos ? shaderSource.size() - 1 : nexLinePos));


			}

			Compile(shaderSources);
		}
		else
		{
			TE_CORE_ERROR("Error: Could not open Shader File [{0}]", path);
		}

	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		TE_PROFILE_PROFILE_SCOPE(m_Path.c_str());

		GLuint program = glCreateProgram();
		glShaderIDs.reserve(shaderSources.size());

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& shaderSrc = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* source = (const GLchar*)shaderSrc.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				char* infoLog = (char*)alloca(maxLength * sizeof(char));
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

				// We don't need the shader anymore.
				glDeleteShader(shader);
				ClearData();

				// Use the infoLog as you see fit.
				TE_ASSERT(false, "Shader compilation error:\n{0}", infoLog);
				// In this simple program, we'll just leave
				return;
			}

			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		m_RendererID = program;
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)alloca(maxLength * sizeof(char));
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog);

			ClearData();

			// Use the infoLog as you see fit.
			TE_ASSERT(false, "Shader link error:\n{0}", infoLog);

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		for (auto& id : glShaderIDs)
		{
			glDetachShader(id, id);
			glDeleteShader(id);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		// We don't need the program anymore.
		ClearData();
	}

	void OpenGLShader::ClearData()
	{
		glDeleteProgram(m_RendererID);

		for (auto& id : glShaderIDs)
			glDeleteShader(id);

		m_UniformLocationCache.clear();
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		auto iterator = m_UniformLocationCache.find(name);
		if (iterator != m_UniformLocationCache.end())
			return (*iterator).second;

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		TE_ASSERT(location >= 0, "Error, uniform not found. \"{0}\" at shader {1}", name, m_Path);

		m_UniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint loc = GetUniformLocation(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);
	}

	void OpenGLShader::SetInt(const std::string& name, const int& integer)
	{
		GLint loc = GetUniformLocation(name);
		glUniform1i(loc, integer);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& floats)
	{
		GLint loc = GetUniformLocation(name);
		glUniform2f(loc, floats[0], floats[1]);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& floats)
	{
		GLint loc = GetUniformLocation(name);
		glUniform3f(loc, floats[0], floats[1], floats[2]);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& floats)
	{
		GLint loc = GetUniformLocation(name);
		glUniform4f(loc, floats[0], floats[1], floats[2], floats[3]);
	}
	
	void OpenGLShader::SetIntArray(const std::string& name, const int* values, const uint32_t count)
	{
		GLint loc = GetUniformLocation(name);
		glUniform1iv(loc, count, values);
	}
}