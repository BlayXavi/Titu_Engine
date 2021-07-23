#pragma once

#ifdef TE_PLATFORM_WINDOWS

TituEngine::Application* TituEngine::CreateApplication();

int main(int argc, char** argv)
{
	TituEngine::Log::Init();

	TituEngine::Application* app = TituEngine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif // TE_PLATFORM_WINDOWS
