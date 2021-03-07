#include "tepch.h"
#include "Application.h"

#include "GLFW/glfw3.h"

namespace TituEngine
{
	Application::Application()
	{
#define BIND_EVENT_FN(x) std::bind(&Application::##x, this, std::placeholders::_1)

		m_Window = std::unique_ptr< Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		TE_CORE_TRACE(e.ToString());

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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack) //compiler undertand it because of implementation of begin() & end()
			{
				layer->OnUpdate();
			}

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
