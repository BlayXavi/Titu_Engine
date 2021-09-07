#include "tepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace TituEngine
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size, bool isStatic)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size, isStatic);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [VertexBuffer]"); return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* vertices, uint32_t count, bool isStatic)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(vertices, count, isStatic);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [VertexBuffer]"); return nullptr;
	}
}
