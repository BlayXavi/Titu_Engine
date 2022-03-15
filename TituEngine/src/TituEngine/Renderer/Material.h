#pragma once

#include "Shader.h"
#include "Texture.h"

namespace TituEngine
{
	class Material
	{
	public:
		virtual ~Material();

		static Material* Create(Shader* shader);
		void Bind(const Shader* overrideShader = nullptr) const;
		void UnBind() const;
		Shader* GetShader() const { return m_Shader; }

		const std::vector<Texture2D*>& GetTextures() const { return m_Textures; }

	private:
		Material(Shader* shader);
		void OnShaderRecompiled();

		void RefreshTextureReferences();

	private:
		Shader* m_Shader = nullptr;
		std::vector<Texture2D*> m_Textures;
		uint32_t m_OnShaderRecompiledHandler;
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