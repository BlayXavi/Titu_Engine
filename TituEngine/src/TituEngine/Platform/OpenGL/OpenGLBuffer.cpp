#include "tepch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace TituEngine
{
	//------------------------------------VERTEX-----------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint size)
	{
		glCreateBuffers(1, &m_RendererID); //also does binding
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//------------------------------------INDEX-----------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint* indices, uint count)
		:m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID); //also does binding
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
