#pragma once
#include "TituEngine/Renderer/Buffer.h"

namespace TituEngine
{
	class VertexArray
	{
	public:
		VertexArray() {}
		virtual  ~VertexArray() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const VertexBuffer* vertexBuffer) = 0;
		virtual void SetIndexBuffer(const IndexBuffer* indexBuffer) = 0;

		static VertexArray* Create();
	};
}