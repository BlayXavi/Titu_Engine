#pragma once

#include "Shader.h"
#include "Texture.h"

namespace TituEngine
{
	class Material
	{
	public:
		static Material* Create(Shader* shader);
		void Bind() const;
		void UnBind() const;
		Shader* GetShader() const { return m_Shader; }
		std::vector<Texture2D*>& GetTextures() { return m_Textures; }

	private:
		Material(Shader* shader);

	private:
		Shader* m_Shader = nullptr;
		std::vector<Texture2D*> m_Textures;
	};

	class MaterialUtilities
	{
	public:
		static Material* s_DefaultMaterial;
		static void Init();
		static void Shutdown();
	private:
		static bool s_Initialized;
	};
}