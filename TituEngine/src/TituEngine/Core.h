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

#ifdef TE_ENABLE_ASSERTS
#define TE_ASSERT(x, ...) {if(!(x)){TE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define TE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)
using uint = unsigned int;