#include <TituEngine.h>

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

TituEngine::Application* TituEngine::CreateApplication()
{
	return new Sandbox();
}
