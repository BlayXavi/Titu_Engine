#pragma once

#ifdef  TE_PLATFORM_WINDOWS
	#ifdef  TE_BUILD_DLL
		#define TE_API __declspec(dllexport)
	#else
		#define TE_API __declspec(dllimport)
	#endif //  TE_BUILD_DLL
#else
	#error TituEngine only supports windows!
#endif //  TE_PLATFORM_WINDOWS
