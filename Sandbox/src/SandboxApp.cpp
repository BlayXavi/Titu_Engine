#include <TituEngine.h>

class SandboxLayer : public TituEngine::Layer
{
public:
	SandboxLayer() : Layer("SBLayer") {}

	void OnUpdate() override
	{
		if (TituEngine::InputBridge::IsKeyPressed(TE_KEY_TAB))
			TE_CLIENT_TRACE("Tab is pressed.");
	}
};

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{

	}

};	

TituEngine::Application* TituEngine::CreateApplication()
{
	return new Sandbox();
}
