#include "tepch.h"
#include "TituEngine/Renderer/RendererUtilities.h"
#include "OpenGLFramebuffer.h"
#include "glad/glad.h"
namespace TituEngine
{
	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, size_t count, uint32_t* outID)
	{
		glCreateTextures(TextureTarget(multisampled), (GLsizei)count, (GLuint*)outID);
	}

	static void BindTexture(bool multisampled, GLuint id)
	{
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(GLuint id, int samples, GLenum internalFormat, GLenum format, GLenum type, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;

		GLenum target = TextureTarget(multisampled);

		if (multisampled)
			glTexImage2DMultisample(target, samples, internalFormat, width, height, GL_FALSE);
		else
		{

			glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, nullptr);
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

	bool IsDepthFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8:	return true;
		}

		return false;
	}

	GLenum FBTexFormatToGLInternalFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER_32:	return GL_R32I;
		}

		TE_ASSERT(false, "Texture format not supported.");
		return 0;
	}

	GLenum FBTexFormatToGLFormat(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:			return GL_RGBA;
		case FramebufferTextureFormat::RED_INTEGER_32:	return GL_RED_INTEGER;
		}

		TE_ASSERT(false, "Texture format not supported.");
		return 0;
	}

	GLenum FBTexFormatToGLType(const FramebufferTextureFormat& format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:			return GL_UNSIGNED_BYTE;
		case FramebufferTextureFormat::RED_INTEGER_32:	return GL_INT;
		}

		TE_ASSERT(false, "Texture format not supported.");
		return 0;
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

	const uint32_t OpenGLFramebuffer::GetPixel(uint32_t attachmentIndex, int x, int y)
	{
		TE_PROFILE_PROFILE_FUNC();
		
		TE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "[Framebuffer] Error, attachment index out of bounds.");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		FramebufferTextureFormat format = m_ColorTextureFormats[attachmentIndex];
		glReadPixels(x, y, 1, 1, FBTexFormatToGLFormat(format), FBTexFormatToGLType(format), &pixelData);
		return pixelData;
	}

	const void OpenGLFramebuffer::ClearAttachment(uint32_t index, int value)
	{
		FramebufferTextureFormat format = m_ColorTextureFormats[index];
		glClearTexImage(m_ColorAttachments[index], 0, FBTexFormatToGLFormat(format), FBTexFormatToGLType(format), &value);
	}

	void OpenGLFramebuffer::DeleteAttachments()
	{
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((GLsizei)m_ColorAttachments.size(), (GLuint*)&m_ColorAttachments[0]);
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
				FramebufferTextureFormat format = m_ColorTextureFormats[i];
				AttachColorTexture(m_ColorAttachments[i], m_FramebufferSpecs.Samples, 
					FBTexFormatToGLInternalFormat(format), FBTexFormatToGLFormat(format), FBTexFormatToGLType(format),
					m_FramebufferSpecs.Width, m_FramebufferSpecs.Height, (uint32_t)i);
			}
		}

		if (m_DepthTextureFormat != FramebufferTextureFormat::NONE)
		{
			CreateTextures(multisample, 1, &m_DepthAttachment);
			BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthTextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(m_DepthAttachment, m_FramebufferSpecs.Samples, 
					GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, //TODO [OPENGL Framebuffer]: Convert it from format
					m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
				break;
			}

		}

		if (m_ColorAttachments.size() > 1)
		{
			TE_ASSERT(m_ColorAttachments.size() <= 4, "Error, m_ColorAttachments must be <= 4.");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((GLsizei)m_ColorAttachments.size(), buffers); //https://stackoverflow.com/questions/51030120/concept-what-is-the-use-of-gldrawbuffer-and-gldrawbuffers/5103034
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE); //depth pass only
		}

		TE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! ErrorCode {0}", glCheckFramebufferStatus(GL_FRAMEBUFFER));

		UnBind();

		OnViewportResized.Dispatch(m_FramebufferSpecs.Width, m_FramebufferSpecs.Height);
	}
}
