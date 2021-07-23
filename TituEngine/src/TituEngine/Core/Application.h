#pragma once

#include "Core.h"
#include "Window.h"
#include "Timestep.h"
#include "TituEngine/Events/Event.h"
#include "TituEngine/Events/ApplicationEvent.h"
#include "TituEngine/Layer/LayerStack.h"
#include "TituEngine/ImGui/ImGuiLayer.h"

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
		ImGuiLayer* m_ImGuiLayer;

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		float m_CurrentTime;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
