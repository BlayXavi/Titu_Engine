#include "tepch.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"
namespace TituEngine
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& framebufferSpecs) :
		Framebuffer(framebufferSpecs)
	{
		Rebuild();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthStencilAtachment);
			m_RendererID = 0;
		}
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			TE_ASSERT(false, "Error. unconsistend size on OpenGLFrameBuffer H[{0}] W[{1}]", height, width);
			return;
		}

		m_FramebufferSpecs.Width = width;
		m_FramebufferSpecs.Height = height;

		Rebuild();
	}

	void OpenGLFramebuffer::Rebuild()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthStencilAtachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		Bind();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthStencilAtachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthStencilAtachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilAtachment, 0);

		TE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		UnBind();
	}
}
