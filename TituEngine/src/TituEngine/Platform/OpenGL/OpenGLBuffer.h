#pragma once
#include "TituEngine/Renderer/Buffer.h"

namespace TituEngine
{
	//------------------------------------VERTEX-----------------------------------------
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(const void* data, uint size) override;

	private:
		friend VertexBuffer; //only vertex buffer can create an instance of OpenGLVertexBuffer
		OpenGLVertexBuffer(float* vertices, uint size, bool isStatic = true);


		uint m_RendererID;
	};

	//------------------------------------INDEX-----------------------------------------
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override ;
		virtual uint GetCount() const { return m_Count; }
	private:
		friend IndexBuffer;//only vertex buffer can create an instance of OpenGLIndexBuffer
		OpenGLIndexBuffer(uint* indices, uint count, bool isStatic = true);

		uint m_RendererID;
		uint m_Count;
	};
}

