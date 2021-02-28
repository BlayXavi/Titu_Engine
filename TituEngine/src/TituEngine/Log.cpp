#include "Log.h"

namespace TituEngine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_CllientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("TituEngine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_CllientLogger = spdlog::stdout_color_mt("APP");
		s_CllientLogger->set_level(spdlog::level::trace);

		TE_CORE_TRACE("Log Module initialized.");
	}
}
