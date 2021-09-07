#include "tepch.h"
#include "Texture.h"

#include "TituEngine/Core/Core.h"
#include "Renderer.h"

#include "TituEngine/Platform/OpenGL/OpenGLTexture2D.h"

namespace TituEngine
{
	Texture2D* Texture2D::Create(uint32_t width, uint32_t height)
	{
		TE_PROFILE_PROFILE_FUNC();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(width, height);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}

	Texture2D* Texture2D::Create(const std::string& path)
	{
		TE_PROFILE_PROFILE_FUNC();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(path);
		}

		TE_ASSERT(false, "RendererAPI not supported. Context: [Texture2D]"); return nullptr;
	}

	//----------------------------------------- SubTexture2D ----------------------------------------- 

	SubTexture2D::SubTexture2D(Texture2D* texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture), m_SpriteSize(1.0f, 1.0f)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	SubTexture2D::SubTexture2D(Texture2D* texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
		: m_Texture(texture), m_SpriteSize(spriteSize)
	{
		glm::vec2 textureSize = { texture->GetWidth(), texture->GetHeight() };
		glm::vec2 normalizedCellSize = cellSize / textureSize;
		glm::vec2 min = coords * normalizedCellSize;
		glm::vec2 max = min + (normalizedCellSize * spriteSize);

		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}
}