#include "tepch.h"
#include "Application.h"

#include "Input/Input.h"

namespace TituEngine
{
#define BIND_EVENT_FN(x) std::bind(&Application::##x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_CurrentTime(0.0f)
	{
		TE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr< Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowsClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
	}

	void Application::Run()
	{
		while (m_Runing)
		{

			float time = TituEngine::Timestep::GetCurrentTime();
			Timestep ts = m_CurrentTime - time; //same as instancing a float!
			m_CurrentTime = time;

			for (Layer* layer : m_LayerStack) //compiler understand it because of implementation of begin() & end()
				layer->OnUpdate(ts);

			m_ImGuiLayer->BeginRender();
			for (Layer* layer : m_LayerStack) //compiler understand it because of implementation of begin() & end()
				layer->OnImGuiRender();
			m_ImGuiLayer->EndRender();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowsClosed(WindowCloseEvent& e)
	{
		m_Runing = false;
		TE_CORE_INFO(e.ToString());
		return true;
	}

}
