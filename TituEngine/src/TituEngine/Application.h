#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Layer/LayerStack.h"
#include "Events/ApplicationEvent.h"

namespace TituEngine
{
	class TE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowsClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;

		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
