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
	};
}