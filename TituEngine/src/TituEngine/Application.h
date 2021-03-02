#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"

namespace TituEngine
{
	class TE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowsClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
