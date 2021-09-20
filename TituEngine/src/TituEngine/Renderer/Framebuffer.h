#pragma once
#include "TituEngine/Core/Core.h"

namespace TituEngine
{
	struct FramebufferSpecs
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpecs& GetFramebufferSpecs() const { return m_FramebufferSpecs; };
		static Framebuffer* Create(const FramebufferSpecs& framebufferSpecs);

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual const uint32_t GetColorAttachment() const = 0;

	protected:
		uint32_t m_RendererID;
		FramebufferSpecs m_FramebufferSpecs;

		Framebuffer(const FramebufferSpecs& framebufferSpecs);
		virtual void Rebuild() = 0;
	};
}
