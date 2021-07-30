#include "tepch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

namespace TituEngine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: Texture2D(path)
	{
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
		TE_ASSERT(data, "Failed loading image! {0}", path.c_str());

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (m_Channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (m_Channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		TE_ASSERT(internalFormat, "Texture format not supported [{0}]", path);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		//see: https://learnopengl.com/Getting-started/Textures
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint slot) const
	{
		glBindTextureUnit(slot, m_RendererID); 
	}
}
