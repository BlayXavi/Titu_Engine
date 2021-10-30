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

#include "TituEngine/Core/Log.h"

#ifdef TE_ENABLE_ASSERTS
//false to trigger assert
#define TE_ASSERT(x, ...) {if(!(x)){TE_CORE_ERROR("Assertion Failed"); TE_CORE_ERROR(__VA_ARGS__); __debugbreak(); } }
#define TE_ASSERT_NOT_IMPLEMENTED TE_ASSERT(false, "Not implemented yet.");
#else
#define TE_ASSERT(x, ...)
#endif

#define BIT(x) (1<<x)
//#define TE_BIND_EVENT_FN (x) std::bind(&x, this, std::placeholders::_1)
//#define TE_BIND_EVENT_FN (fn) [this](auto&& args...) ->decltype(auto) {this->fn(std::forward<decltype(args)>(args)...));}

#define GENERATE_RANDOM(min, max) min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)))
