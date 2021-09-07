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
		virtual void SetData(const void* data, uint32_t size) override;

	private:
		friend VertexBuffer; //only vertex buffer can create an instance of OpenGLVertexBuffer
		OpenGLVertexBuffer(float* vertices, uint32_t size, bool isStatic = true);


		uint32_t m_RendererID;
	};

	//------------------------------------INDEX-----------------------------------------
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override ;
		virtual uint32_t GetCount() const { return m_Count; }
	private:
		friend IndexBuffer;//only vertex buffer can create an instance of OpenGLIndexBuffer
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count, bool isStatic = true);

		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}

