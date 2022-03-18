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
		virtual const uint32_t GetDepthAttachment() const override { return m_DepthAttachment; }
		virtual const uint32_t GetPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual const std::vector<uint32_t> GetAttachments() override { return m_ColorAttachments; }
		virtual const void BindColorAttachments() override;

		virtual const void ClearAttachment(uint32_t index, int value);

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