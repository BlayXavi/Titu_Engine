#include <TituEngine.h>
#include <TituEngine/Core/EntryPoint.h>
#include "Sandbox2D.h"


using namespace TituEngine;

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2DLayer());
	}
};

TituEngine::Application* TituEngine::CreateApplication()
{
	return new Sandbox();
}
