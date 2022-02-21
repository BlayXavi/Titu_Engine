#include "tepch.h"
#include "Framebuffer.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace TituEngine
{
	Framebuffer::Framebuffer(const FramebufferSpecs& framebufferSpecs)
		: m_FramebufferSpecs(framebufferSpecs), m_RendererID(0)
	{

	}

	Framebuffer* Framebuffer::Create(const FramebufferSpecs& framebufferSpecs)
	{
		TE_PROFILE_PROFILE_FUNC();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLFramebuffer(framebufferSpecs);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Framebuffer]"); return nullptr;
	}

	void Framebuffer::SetDirty(const uint32_t& width, const uint32_t& height)
	{
		m_DirtySize[0] = width;
		m_DirtySize[1] = height;
		m_Dirty = true;
	}

	void Framebuffer::ProcessDirty()
	{
		if (m_Dirty)
		{
			m_Dirty = false;
			Resize(m_DirtySize[0], m_DirtySize[1]);
		}
	}
}