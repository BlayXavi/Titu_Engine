#pragma once

#include <string>
#include "TituEngine/Core.h"

namespace TituEngine
{

	class Texture
	{
	public:
		inline uint GetWidth() const { return m_Width; }
		inline uint GetHeight() const { return m_Height; }
		inline std::string GetPath() const { return m_Path; }
		virtual void Bind(uint slot = 0) const = 0;

	protected:
		Texture(const std::string& path) : m_Path(path), m_Width(0), m_Height(0), m_Channels(0) { }

		std::string m_Path;

		int m_Width;
		int m_Height;
		int m_Channels;
	};

	class Texture2D : public Texture 
	{
	public:
		static Texture2D* Create(const std::string& path);

	protected:
		Texture2D(const std::string& path) : Texture(path) { }
	};

}