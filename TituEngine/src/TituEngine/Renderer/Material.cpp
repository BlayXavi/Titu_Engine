#include "tepch.h"
#include "Material.h"

namespace TituEngine
{
	Material::~Material()
	{
		if (m_Shader != nullptr)
			m_Shader->ShaderRecompiled.RemoveListener(m_OnShaderRecompiledHandler);
	}

	Material* Material::Create(Shader* shader)
	{
		return new Material(shader);;
	}

	void Material::Bind(const Shader* overrideShader) const
	{
		if (overrideShader == nullptr)
			m_Shader->Bind();
		else
			overrideShader->Bind();

		for (size_t i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Bind(i);
	}

	void Material::UnBind() const
	{
		m_Shader->Unbind();
	}

	Material::Material(Shader* shader)
	{
		if (m_Shader != nullptr)
			m_Shader->ShaderRecompiled.RemoveListener(m_OnShaderRecompiledHandler);

		m_Shader = shader;
		m_OnShaderRecompiledHandler = m_Shader->ShaderRecompiled.AddListener(this, &Material::OnShaderRecompiled);

		RefreshTextureReferences();
	}

	void Material::OnShaderRecompiled()
	{
		RefreshTextureReferences();
	}

	void Material::RefreshTextureReferences()
	{
		int currentTexturesCount = m_Textures.size();

		m_Textures.resize(m_Shader->GetTextureResourcesCount());

		for (size_t i = currentTexturesCount; i < m_Textures.size(); i++)
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
