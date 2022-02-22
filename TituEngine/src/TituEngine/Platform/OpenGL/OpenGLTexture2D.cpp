#include "tepch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

namespace TituEngine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: Texture2D(width, height), m_DataFormat(0), m_InternalFormat(0)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		//see: https://learnopengl.com/Getting-started/Textures
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: Texture2D(path), m_DataFormat(0), m_InternalFormat(0)
	{
		TE_PROFILE_PROFILE_FUNC();

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		{
			TE_PROFILE_PROFILE_SCOPE("stbi_load");
			data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		}
		if (!data)
		{
			TE_ASSERT(data, "Failed loading image! {0}", path.c_str());
		}
		else
		{
			if (m_Channels == 4)
			{
				m_InternalFormat = GL_RGBA8;
				m_DataFormat = GL_RGBA;
			}
			else if (m_Channels == 3)
			{
				m_InternalFormat = GL_RGB8;
				m_DataFormat = GL_RGB;
			}
			else if (m_Channels == 2)
			{
				m_InternalFormat = GL_RG8;
				m_DataFormat = GL_RG;
			}

			TE_ASSERT(m_InternalFormat, "Texture format not supported [{0}]", path);

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

			//see: https://learnopengl.com/Getting-started/Textures
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			SetData(data, m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3));
		}
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		TE_PROFILE_PROFILE_FUNC();
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size, bool deleteData)
	{
		TE_PROFILE_PROFILE_FUNC();
		TE_ASSERT(size == m_Width * m_Height * (m_DataFormat == GL_RGBA ? 4 : 3), "Error, size does not match with m_Width && m_Height.");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		if (deleteData)
			delete data;
	}
}
