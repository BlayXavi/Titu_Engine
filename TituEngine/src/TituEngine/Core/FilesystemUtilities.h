#pragma once

#include <filesystem>

namespace TituEngine
{
	static const std::filesystem::path s_DefaultAssetPath = "assets";
	static const std::string s_TituSceneExtension = ".tituscene";
	static const std::string s_PNGExtension = ".png";
	static const std::string s_JPGExtension = ".jpg";
	static const std::string s_ObjExtension = ".obj";
	static const std::string s_FBXExtension = ".fbx";
	static const std::string s_ShaderExtension = ".glsl";

	static const std::filesystem::path s_DefaultAssetPath_Texture2D = s_DefaultAssetPath / "textures2D";
	static const std::filesystem::path s_DefaultAssetPath_Mesh = s_DefaultAssetPath / "meshes";
}