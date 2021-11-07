#pragma once
#pragma once

#include <string>

namespace TituEngine
{
	class FileDialogs
	{
	public:
		//Extension Filter Format: "Pretty Name (*.extension)\\0.extension\\0"
		static std::string OpenFile(const char* filter);
		//Extension Filter Format: "Pretty Name (*.extension)\\0.extension\\0"
		static std::string SaveFile(const char* filter);
	};
}