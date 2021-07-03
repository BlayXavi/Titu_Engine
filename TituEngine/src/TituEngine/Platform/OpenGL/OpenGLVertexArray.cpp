#include "tepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace TituEngine
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		m_IndexBuffer = nullptr;
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer* vertexBuffer)
	{
		TE_ASSERT(vertexBuffer->GetLayout().GetElements().size() != 0, "VertexBuffer not Initialized on AddVertexBuffer");

		Bind();
		vertexBuffer->Bind();
		uint index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.ElementCount, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer* indexBuffer)
	{
		Bind();
		m_IndexBuffer = indexBuffer;
		m_IndexBuffer->Bind();
	}
}
