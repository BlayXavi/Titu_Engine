#include "tepch.h"
#include "Material.h"

namespace TituEngine
{
	Material* Material::Create(Shader* shader)
	{
		return new Material(shader);;
	}

	void Material::Bind() const
	{
		m_Shader->Bind();
		for (size_t i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Bind();
	}

	void Material::UnBind() const
	{
		m_Shader->Unbind();
	}

	Material::Material(Shader* shader)
	{
		m_Shader = shader;
		m_Textures.resize(m_Shader->GetTextureResourcesCount());
		for (size_t i = 0; i < m_Textures.size(); i++)
			m_Textures[i] = TextureUtilities::s_WhiteTexture->GetTexture();
	}

	// ----------------- MATERIAL UTILITIES ----------------
	Material* MaterialUtilities::s_DefaultMaterial = nullptr;
	bool MaterialUtilities::s_Initialized = false;

	void MaterialUtilities::Init()
	{
		if (s_Initialized)
			return;

		s_Initialized = true;
		s_DefaultMaterial = Material::Create(ShaderUtilities::s_DefaultShader);
	}

	void MaterialUtilities::Shutdown()
	{
		if (!s_Initialized)
			return;
		delete s_DefaultMaterial;
	}
}
