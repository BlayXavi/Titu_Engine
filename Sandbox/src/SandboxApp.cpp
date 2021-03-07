#include <TituEngine.h>

class ExampleLayer : public TituEngine::Layer
{
public:
	ExampleLayer() : Layer("Example") {};
	void OnUpdate() override
	{
		TE_CLIENT_INFO("ExampleLayer::OnUpdate()");
	}

	void OnEvent(TituEngine::Event& e)
	{
		TE_CLIENT_TRACE("ExampleLayer event received: {0}", e.ToString());
	}
};

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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
