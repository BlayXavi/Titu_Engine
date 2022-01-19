#include "tepch.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"
namespace TituEngine
{
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, uint32_t count, uint32_t* outID)
	{
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}

	static void BindTexture(bool multisampled, GLuint id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(GLuint id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;

		GLenum target = TextureTarget(multisampled);

		if (multisampled)
			glTexImage2DMultisample(target, samples, format, width, height, GL_FALSE);
		else
		{

			glTexImage2D(target, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, id, 0);
	}

	static void AttachDepthTexture(GLuint id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;

		GLenum target = TextureTarget(multisampled);

		if (multisampled)
			glTexImage2DMultisample(target, samples, format, width, height, GL_FALSE);
		else
		{

			glTexStorage2D(target, 1, format, width, height);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, target, id, 0);
	}

	bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		}

		return false;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecs& framebufferSpecs) :
		Framebuffer(framebufferSpecs)
	{
		for (FramebufferTextureFormat format : m_FramebufferSpecs.Attachments)
		{
			if (!IsDepthFormat(format))
			{
				m_ColorTextureFormats.emplace_back(format);
			}
			else
			{
				m_DepthTextureFormat = format;
			}
		}
		Rebuild();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		DeleteAttachments();
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

	void OpenGLFramebuffer::DeleteAttachments()
	{
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), &m_ColorAttachments[0]);
			glDeleteTextures(1, &m_DepthAttachment);
			m_RendererID = 0;
		}
	}

	void OpenGLFramebuffer::Rebuild()
	{
		DeleteAttachments();

		glCreateFramebuffers(1, &m_RendererID);
		Bind();

		bool multisample = m_FramebufferSpecs.IsMultiSampled();

		if (m_ColorTextureFormats.size() > 0)
		{
			m_ColorAttachments.resize(m_ColorTextureFormats.size());
			CreateTextures(multisample, m_ColorAttachments.size(), &m_ColorAttachments[0]);
			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorTextureFormats[i])
				{
				case FramebufferTextureFormat::RBA8:
					AttachColorTexture(m_ColorAttachments[i], m_FramebufferSpecs.Samples, GL_RGBA8, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, i);
					break;
				}
			}
		}

		if (m_DepthTextureFormat != FramebufferTextureFormat::NONE)
		{
			CreateTextures(multisample, 1, &m_DepthAttachment);
			BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthTextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, m_FramebufferSpecs.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
				break;
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			TE_ASSERT(m_ColorAttachments.size() <= 4, "Error, m_ColorAttachments must be <= 4.");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers); //https://stackoverflow.com/questions/51030120/concept-what-is-the-use-of-gldrawbuffer-and-gldrawbuffers/5103034
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE); //depth pass only
		}

		TE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! ErrorCode {0}", glCheckFramebufferStatus(GL_FRAMEBUFFER));

		UnBind();
	}
}
