#pragma once

#include <filesystem>
#include "TituEngine/Renderer/Texture.h"

namespace TituEngine
{
	class ContentBrowserPanel
	{

	public:
		ContentBrowserPanel();
		void OnImGuiRender(bool& showWindow);

	private:
		void DrawDirectoryTree(std::string directory);

	private:
		std::filesystem::path m_CurrentPath;

		Texture2D* m_DirectoryIcon = nullptr;
		Texture2D* m_FileIcon = nullptr;

		float m_ThumbnailSize = 16.0f;
	};
}