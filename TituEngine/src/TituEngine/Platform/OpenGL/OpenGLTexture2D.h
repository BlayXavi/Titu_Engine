#pragma once

#include "TituEngine/Renderer/Texture.h"
#include "glad/glad.h"

namespace TituEngine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);

		void Bind(uint slot = 0) const override;

		~OpenGLTexture2D();
	private:
		GLuint m_RendererID;
	};
}