#include "tepch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace TituEngine
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// https://www.khronos.org/opengl/wiki/Shader_Compilation

		// Create an empty vertex shader handle
		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(m_VertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(m_VertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)alloca(maxLength * sizeof(char));
			glGetShaderInfoLog(m_VertexShader, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(m_VertexShader);

			// Use the infoLog as you see fit.
			TE_ASSERT(false, "Vertex shader compilation error:\n{0}", infoLog);
			// In this simple program, we'll just leave
			return;
		}

		// Create an empty fragment shader handle
		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(m_FragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(m_FragmentShader);

		glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(m_FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)alloca(maxLength * sizeof(char));
			glGetShaderInfoLog(m_FragmentShader, maxLength, &maxLength, infoLog);

			// We don't need the shader anymore.
			glDeleteShader(m_FragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(m_VertexShader);

			// Use the infoLog as you see fit.
			TE_ASSERT(false, "Fragment Shader error\n{0}", infoLog);

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_Program = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_Program, m_VertexShader);
		glAttachShader(m_Program, m_FragmentShader);

		// Link our program
		glLinkProgram(m_Program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_Program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = (char*)alloca(maxLength * sizeof(char));
			glGetProgramInfoLog(m_Program, maxLength, &maxLength, infoLog);

			// We don't need the program anymore.
			glDeleteProgram(m_Program);
			// Don't leak shaders either.
			glDeleteShader(m_VertexShader);
			glDeleteShader(m_FragmentShader);

			// Use the infoLog as you see fit.
			TE_ASSERT(false, "Shader link error:\n{0}", infoLog);

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_Program, m_VertexShader);
		glDetachShader(m_Program, m_FragmentShader);
	}

	Shader::~Shader()
	{
		// We don't need the program anymore.
		glDeleteProgram(m_Program);
		// Don't leak shaders either.
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}