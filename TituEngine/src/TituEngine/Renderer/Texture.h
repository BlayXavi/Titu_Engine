#pragma once

#include <string>
#include "TituEngine/Core/Core.h"
#include <glm/vec2.hpp>

namespace TituEngine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline std::string GetPath() const { return m_Path; }

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size, bool deleteData = false) = 0;


		virtual bool operator==(const Texture& other) const = 0;

	protected:
		Texture(const std::string& path) : m_Path(path), m_Width(0), m_Height(0), m_Channels(0) { }
		Texture(uint32_t width, uint32_t height) : m_Path(""), m_Width(width), m_Height(height), m_Channels(0) { }

		std::string m_Path;

		int m_Width;
		int m_Height;
		int m_Channels;
	};

	class Texture2D : public Texture
	{
	public:
		static Texture2D* Create(uint32_t width, uint32_t height);
		static Texture2D* Create(const std::string& path);

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size, bool deleteData = false) = 0;

	protected:
		Texture2D(const std::string& path) : Texture(path) { }
		Texture2D(uint32_t width, uint32_t height) : Texture(width, height) { }
	};


	class SubTexture2D
	{
	public:
		SubTexture2D() = default;
		SubTexture2D(Texture2D* texture, const glm::vec2& min, const glm::vec2& max);
		SubTexture2D(Texture2D* texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize);

		Texture2D* GetTexture() const { return m_Texture; }
		glm::vec2* GetTexCoords() { return m_TexCoords; }
		glm::vec2 GetSpriteSize() { return m_SpriteSize; }

	private:
		Texture2D* m_Texture;
		glm::vec2 m_TexCoords[4];
		glm::vec2 m_SpriteSize;
	};
}