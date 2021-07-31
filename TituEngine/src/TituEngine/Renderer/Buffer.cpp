#include "tepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLBuffer.h"

namespace TituEngine
{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint size)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [VertexBuffer]"); return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint* vertices, uint count)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(vertices, count);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [VertexBuffer]"); return nullptr;
	}
}
