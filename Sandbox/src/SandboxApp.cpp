#include <TituEngine.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

class SandboxLayer : public TituEngine::Layer
{
public:
	SandboxLayer() : Layer("SBLayer") 
	{
	}

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
