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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Instance() { return *s_Instance; }

	private:
		bool OnWindowsClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;

		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
