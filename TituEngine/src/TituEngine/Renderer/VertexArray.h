#pragma once
#include "TituEngine/Renderer/Buffer.h"

namespace TituEngine
{
	class VertexArray
	{
	public:
		VertexArray() {}
		virtual  ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IndexBuffer* indexBuffer) = 0;

		virtual inline const std::vector<const VertexBuffer*> GetVertexBuffers() const = 0;
		virtual inline const IndexBuffer* GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}