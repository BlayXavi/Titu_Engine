#pragma once

#include "TituEngine/Renderer/Texture.h"
#include "glad/glad.h"

namespace TituEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		void Bind(uint slot = 0) const override;

		~OpenGLTexture2D();

	private:
		OpenGLTexture2D(const std::string& path);
		GLuint m_RendererID;

		friend Texture2D;
	};
}