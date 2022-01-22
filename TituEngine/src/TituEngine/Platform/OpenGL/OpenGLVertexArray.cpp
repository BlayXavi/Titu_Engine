#include "tepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace TituEngine
{
	OpenGLVertexArray::OpenGLVertexArray() : m_VertexBufferIndexOffset(0)
	{
		m_IndexBuffer = nullptr;
		glCreateVertexArrays(1, &m_VertexArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_VertexArrayID);
		for (auto& vb : m_VertexBuffers)
		{
			delete vb;
			vb = nullptr;
		}
		delete m_IndexBuffer;
		m_IndexBuffer = nullptr;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer* vertexBuffer)
	{
		TE_ASSERT(vertexBuffer->GetLayout().GetElements().size() != 0, "VertexBuffer not Initialized on AddVertexBuffer");

		Bind();
		vertexBuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(index + m_VertexBufferIndexOffset);

			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				glVertexAttribPointer(index + m_VertexBufferIndexOffset, element.ElementCount, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(intptr_t)element.Offset);
				index++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			{
				glVertexAttribIPointer(index + m_VertexBufferIndexOffset, element.ElementCount, GL_INT, layout.GetStride(), (const void*)(intptr_t)element.Offset);
				index++;
				break;
			}

			default:
				break;
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += (uint32_t)vertexBuffer->GetLayout().GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer* indexBuffer)
	{
		Bind();
		m_IndexBuffer = indexBuffer;
		m_IndexBuffer->Bind();
	}
}
