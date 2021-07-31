#pragma once

#include <string>
#include "TituEngine/Core/Core.h"

namespace TituEngine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }
		inline std::string GetPath() const { return m_Path; }

		virtual void Bind(uint slot = 0) const = 0;
		virtual void SetData(void* data, uint size, bool deleteData = false) = 0;

	protected:
		Texture(const std::string& path) : m_Path(path), m_Width(0), m_Height(0), m_Channels(0){ }
		Texture(uint width, uint height) : m_Path(""), m_Width(width), m_Height(height), m_Channels(0) { }

		std::string m_Path;

		int m_Width;
		int m_Height;
		int m_Channels;
	};

	class Texture2D : public Texture
	{
	public:
		static Texture2D* Create(uint width, uint height);
		static Texture2D* Create(const std::string& path);

		virtual void Bind(uint slot = 0) const = 0;
		virtual void SetData(void* data, uint size, bool deleteData = false) = 0;

	protected:
		Texture2D(const std::string& path) : Texture(path) { }
		Texture2D(uint width, uint height) : Texture(width, height) { }
	};

}