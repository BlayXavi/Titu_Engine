#pragma once

#include <filesystem>

namespace TituEngine
{
	static const std::filesystem::path s_DefaultAssetPath = "assets";
	static const std::string m_TituSceneExtension = ".tituscene";
	static const std::string m_PNGExtension = ".png";

	static const std::filesystem::path s_DefaultAssetPath_Texture2D = s_DefaultAssetPath / "textures2D";
	static const std::filesystem::path s_DefaultAssetPath_Mesh = s_DefaultAssetPath / "meshes";
}