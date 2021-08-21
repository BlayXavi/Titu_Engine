#pragma once

#include "TituEngine/Renderer/Texture.h"
#include "glad/glad.h"

namespace TituEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		void Bind(uint slot = 0) const override;
		void SetData(void* data, uint size, bool deleteData = false) override;

		~OpenGLTexture2D();

		virtual bool operator ==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
		}


	private:
		friend Texture2D;

		OpenGLTexture2D(uint width, uint height);
		OpenGLTexture2D(const std::string& path);
	
	private:

		GLenum m_DataFormat;
		GLenum m_InternalFormat;
		GLuint m_RendererID;
	};
}