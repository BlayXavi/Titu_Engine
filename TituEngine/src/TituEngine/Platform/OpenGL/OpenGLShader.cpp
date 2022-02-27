#include "tepch.h"
#include "OpenGLShader.h"
#include "TituEngine/Core/Core.h"

#include <fstream>
#include <glad/glad.h>

#include <filesystem>

namespace TituEngine
{

	std::string OpenGLShader::s_CacheDirectory = "assets/cache/shader/opengl";

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		TE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	static shaderc_shader_kind GLShaderStageToShaderc(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}
		TE_ASSERT(false, "No Shader type recognized!");
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageToString(GLenum stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}
		TE_ASSERT(false, "No Shader type recognized!");
		return nullptr;
	}

	static void CreateCacheDirectoryIfNeeded()
	{
		std::string cacheDirectory = OpenGLShader::GetCacheDirectory();
		if (!std::filesystem::exists(cacheDirectory))
			std::filesystem::create_directories(cacheDirectory);
	}

	static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
		}
		TE_ASSERT(false, "No Shader type recognized!");
		return "";
	}

	static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
	{
		switch (stage)
		{
		case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
		case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
		}
		TE_ASSERT(false, "No Shader type recognized!");
		return "";
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		TE_PROFILE_PROFILE_FUNC();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				TE_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			TE_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::SplitStages(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			TE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			TE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			TE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}


	OpenGLShader::OpenGLShader(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		m_Path = path;
		m_Name = path.substr(path.find_first_of("\\"), path.find_last_of("."));

		CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(m_Path);
		std::unordered_map<GLenum, std::string> shaderStages = SplitStages(source);

		m_CompilationSucceeded= CompileOrGetVulkanBinaries(shaderStages);
		m_CompilationSucceeded = m_CompilationSucceeded && CompileOrGetOpenGLBinaries();
		if(m_CompilationSucceeded)
			CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		// We don't need the program anymore.
		ClearData();
	}

	void OpenGLShader::Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		m_ShaderResources = compiler.get_shader_resources();

		TE_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", GLShaderStageToString(stage), m_Path);
		TE_CORE_TRACE("\t {0} uniform buffers", m_ShaderResources.uniform_buffers.size());
		TE_CORE_TRACE("\t {0} resources", m_ShaderResources.sampled_images.size());

		if (m_ShaderResources.uniform_buffers.size() > 0)
		{
			TE_CORE_TRACE("Uniform buffers:");
			int index = 0;
			spirv_cross::ShaderResources res = compiler.get_shader_resources();
			for (const auto& resource : res.uniform_buffers)
			{
				const auto& bufferType = compiler.get_type(resource.base_type_id);
				size_t bufferSize = compiler.get_declared_struct_size(bufferType);
				size_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
				size_t memberCount = bufferType.member_types.size();

				TE_CORE_TRACE("\t {0}", resource.name);
				TE_CORE_TRACE("\t\t Size = {0}", bufferSize);
				TE_CORE_TRACE("\t\t Binding = {0}", binding);
				TE_CORE_TRACE("\t\t Members = {0}", memberCount);
				index++;
			}
		}

		if (m_ShaderResources.storage_buffers.size() > 0)
		{
			TE_CORE_TRACE("Storage buffers:");
			spirv_cross::ShaderResources res = compiler.get_shader_resources();
			for (const auto& resource : res.storage_buffers)
			{
				const auto& bufferType = compiler.get_type(resource.base_type_id);
				size_t bufferSize = compiler.get_declared_struct_size(bufferType);
				size_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
				size_t memberCount = bufferType.member_types.size();
				std::string name = compiler.get_name(resource.base_type_id);

				TE_CORE_TRACE("\t {0}", resource.name);
				TE_CORE_TRACE("\t {0}", name);
				TE_CORE_TRACE("\t\t Size = {0}", bufferSize);
				TE_CORE_TRACE("\t\t Binding = {0}", binding);
				TE_CORE_TRACE("\t\t Members = {0}", memberCount);
			}
		}
	}

	bool OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<uint32_t, std::string>& shaderSources)
	{
		shaderc::Compiler shaderCompiler;
		shaderc::CompileOptions shaderCompilerOptions;

		shaderCompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			shaderCompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_VulkanSPIRVStages.clear();

		std::string cacheDir = GetCacheDirectory();

		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_Path;
			std::filesystem::path cachedPath = cacheDir + "\\" + (shaderFilePath.filename().string() + GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) //if Vulkan stages are compiled and saved on disk load it
			{
				in.seekg(0, std::ios::end);
				std::streampos size = in.tellg();
				in.seekg(0, std::ios::beg);

				std::vector<uint32_t>& data = m_VulkanSPIRVStages[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);

				in.close();
			}
			else //if vulkan stages are NOT compiled, compile it from source stages
			{
				shaderc::SpvCompilationResult module = shaderCompiler.CompileGlslToSpv(source, GLShaderStageToShaderc(stage), m_Path.c_str(), shaderCompilerOptions);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					TE_CORE_ERROR(module.GetErrorMessage());
					return false;
				}
				else
				{
					m_VulkanSPIRVStages[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());
					std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
					if (out.is_open())
					{
						auto& data = m_VulkanSPIRVStages[stage];
						out.write((char*)data.data(), data.size() * sizeof(uint32_t));
						out.flush();
						out.close();
					}
				}
			}
		}

		for (auto&& [stage, data] : m_VulkanSPIRVStages)
			Reflect(stage, data);

		return true;

	}

	bool OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		shaderc::Compiler shaderCompiler;
		shaderc::CompileOptions shaderCompilerOptions;

		shaderCompilerOptions.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = true;
		if (optimize)
			shaderCompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

		m_OpenGLSPIRVStages.clear();

		std::string cacheDir = GetCacheDirectory();

		for (auto&& [stage, spirv] : m_VulkanSPIRVStages)
		{
			std::filesystem::path shaderFilePath = m_Path;
			std::filesystem::path cachedPath = cacheDir + "\\" + (shaderFilePath.filename().string() + GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open()) //get cached binnaries
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = m_OpenGLSPIRVStages[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else //compile shader source
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				std::string& openGlSourceCode = glslCompiler.compile();

				shaderc::SpvCompilationResult module = shaderCompiler.CompileGlslToSpv(openGlSourceCode, GLShaderStageToShaderc(stage), m_Path.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					TE_CORE_ERROR(module.GetErrorMessage());
					return false;
				}
				else
				{
					m_OpenGLSPIRVStages[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

					std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
					if (out.is_open())
					{
						auto& data = m_OpenGLSPIRVStages[stage];
						out.write((char*)data.data(), data.size() * sizeof(uint32_t));
						out.flush();
						out.close();
					}
				}
			}
		}

		for (auto&& [stage, data] : m_OpenGLSPIRVStages)
			Reflect(stage, data);

		return true;
	}

	void OpenGLShader::CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRVStages)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), (GLsizei)spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			TE_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_Path, infoLog.data());

			glDeleteProgram(program);

			for (GLuint shaderID : shaderIDs)
				glDeleteShader(shaderID);

		}
		else
		{
			for (GLuint id : shaderIDs)
			{
				glDetachShader(program, id);
				glDeleteShader(id);
			}

			m_RendererID = program;
		}

		m_VulkanSPIRVStages.clear();
		m_OpenGLSPIRVStages.clear();
	}

	void OpenGLShader::ClearData()
	{
		for (GLuint id : glShaderIDs)
		{
			glDetachShader(m_RendererID, id);
			glDeleteShader(id);
		}

		glDeleteProgram(m_RendererID);

		glShaderIDs.clear();

		m_UniformLocationCache.clear();

		m_VulkanSPIRVStages.clear();
		m_OpenGLSPIRVStages.clear();
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