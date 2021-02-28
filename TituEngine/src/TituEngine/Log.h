#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace TituEngine
{
	class TE_API Log
	{
	public:
		static void Init();

		static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_CllientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_CllientLogger;
	};
}

//Core Log macros
#define TE_CORE_INFO(...)			::TituEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_TRACE(...)			::TituEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TE_CORE_WARN(...)			::TituEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_ERROR(...)			::TituEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_CRITICAL(...)		::TituEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client Log macros
#define TE_CLIENT_INFO(...)			::TituEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define TE_CLIENT_TRACE(...)		::TituEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TE_CLIENT_WARN(...)			::TituEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TE_CLIENT_ERROR(...)		::TituEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define TE_CLIENT_CRITICAL(...)		::TituEngine::Log::GetClientLogger()->critical(__VA_ARGS__)