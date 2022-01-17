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

		virtual const uint32_t GetColorAttachment(uint32_t index = 0) const override { return m_ColorAttachments[index]; }

	private:
		friend Framebuffer;
		OpenGLFramebuffer(const FramebufferSpecs& framebufferSpecs);

		std::vector<FramebufferTextureFormat> m_ColorTextureFormats;
		FramebufferTextureFormat m_DepthTextureFormat = FramebufferTextureFormat::NONE;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;

	protected:
		virtual void DeleteAttachments() override;
		virtual void Rebuild() override;
	};
}