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