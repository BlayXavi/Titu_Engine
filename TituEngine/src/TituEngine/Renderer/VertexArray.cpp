#include "tepch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLVertexArray.h"

namespace TituEngine
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [VertexArray]"); return nullptr;
		return nullptr;
	}
}