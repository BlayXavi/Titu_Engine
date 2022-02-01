#include "tepch.h"
#include "UniformBuffer.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLUniformBuffer.h"

namespace TituEngine
{
	UniformBuffer* UniformBuffer::Create(const uint32_t& size, const uint32_t& binding)
	{
		TE_PROFILE_PROFILE_FUNC();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLUniformBuffer(size, binding);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [UniformBuffer]"); return nullptr;
	}
}

