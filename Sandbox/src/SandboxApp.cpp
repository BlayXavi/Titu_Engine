#include <TituEngine.h>

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{
		PushOverlay(new TituEngine::ImGuiLayer());
	}

	~Sandbox()
	{

	}

};

TituEngine::Application* TituEngine::CreateApplication()
{
	return new Sandbox();
}
