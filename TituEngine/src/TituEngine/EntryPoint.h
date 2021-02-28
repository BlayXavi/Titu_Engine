#pragma once

#ifdef TE_PLATFORM_WINDOWS

TituEngine::Application* TituEngine::CreateApplication();

int main(int argc, char** argv)
{
	printf("Titu Engine launched!\n");

	auto app = TituEngine::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif // TE_PLATFORM_WINDOWS
