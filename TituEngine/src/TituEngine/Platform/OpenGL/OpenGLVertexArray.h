#pragma once
#include "TituEngine/Renderer/VertexArray.h"

namespace TituEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) override;
		virtual void SetIndexBuffer(const IndexBuffer* indexBuffer) override;

		virtual inline const std::vector<const VertexBuffer*> GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual inline const IndexBuffer* GetIndexBuffer() const override { return m_IndexBuffer; };

	private:
		friend VertexArray; //Only VertexArray can create instances of OpenGlVertexArray
		OpenGLVertexArray();

		std::vector<const VertexBuffer*> m_VertexBuffers;
		const IndexBuffer* m_IndexBuffer = nullptr;
		uint m_VertexArrayID;
		
		//Buffer Index Tracker. Used to add an offset to Vertex Attrib Array/Pointer
		uint m_VertexBufferIndexOffset;
	};
}