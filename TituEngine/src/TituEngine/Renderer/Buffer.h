#pragma once

#include "TituEngine/Core.h"

namespace TituEngine
{
	class Buffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class VertexBuffer : public Buffer
	{
	public:
		~VertexBuffer() = default;
		static VertexBuffer* Create(float* vertices, uint size);
	};

	class IndexBuffer : public Buffer
	{
	public:
		~IndexBuffer() = default;

		static IndexBuffer* Create(uint* vertices, uint size);
		virtual uint GetCount() const = 0;
	};

}