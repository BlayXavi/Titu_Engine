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
	}

	void Application::Run()
	{
		while (m_Runing)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
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
