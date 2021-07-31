#pragma once

#include "TituEngine/Instrumentation/Profiler.h"

#ifdef TE_PLATFORM_WINDOWS

TituEngine::Application* TituEngine::CreateApplication();

int main(int argc, char** argv)
{
	TituEngine::Log::Init();

	TE_PROFILE_BEGIN_SESSION("App init", "TituEngine_initApp.json");
	TituEngine::Application* app = TituEngine::CreateApplication();
	TE_PROFILE_END_SESSION();

	TE_PROFILE_BEGIN_SESSION("App Run", "TituEngine_runApp.json");
	app->Run();
	TE_PROFILE_END_SESSION();

	TE_PROFILE_BEGIN_SESSION("App Shutdown", "TituEngine_shutdownApp.json");
	delete app;
	TE_PROFILE_END_SESSION();
	return 0;
}

#endif // TE_PLATFORM_WINDOWS
