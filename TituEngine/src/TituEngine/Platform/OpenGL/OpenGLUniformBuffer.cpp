#include "tepch.h"
#include "OpenGLUniformBuffer.h"

#include "glad/glad.h"

namespace TituEngine
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t& size, const uint32_t& binding)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); //GL_DYNAMIC -> Data will be modified almost every frame (at least reset)
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, const uint32_t& size, const uint32_t& offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}