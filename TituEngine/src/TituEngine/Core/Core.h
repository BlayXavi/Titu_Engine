#pragma once

#ifdef _WIN32
#ifdef _WIN64
#define TE_PLATFORM_WINDOWS
#else
error Winx86 not supported
#endif
#else
error Platform not supported
#endif

#ifdef  TE_PLATFORM_WINDOWS
#if TE_DLL
#ifdef  TE_BUILD_DLL
#define TE_API __declspec(dllexport)
#else
#define TE_API __declspec(dllimport)
#endif
#else
#define TE_API
#endif //  TE_BUILD_DLL
#endif //  TE_PLATFORM_WINDOWS

#ifdef TE_ENABLE_ASSERTS
#define TE_ASSERT(x, ...) {if(!(x)){TE_CORE_ERROR("Assertion Failed"); TE_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#else
#define TE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)
#define TE_BIND_EVENT_FN (x) std::bind(&x, this, std::placeholders::_1)

#define GENERATE_RANDOM(min, max) min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)))
