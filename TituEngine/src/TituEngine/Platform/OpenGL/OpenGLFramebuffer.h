#pragma once
#include "TituEngine/Renderer/Framebuffer.h"

namespace TituEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual const uint32_t GetColorAttachment() const override { return m_ColorAttachment; }

	private:
		friend Framebuffer;
		OpenGLFramebuffer(const FramebufferSpecs& framebufferSpecs);

		uint32_t m_ColorAttachment;
		uint32_t m_DepthStencilAtachment;

	protected:
		virtual void Rebuild() override;
	};
}