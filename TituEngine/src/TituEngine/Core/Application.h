#pragma once

#include "Window.h"
#include "Timestep.h"
#include "TituEngine/Events/Event.h"
#include "TituEngine/Events/ApplicationEvent.h"
#include "TituEngine/Layer/LayerStack.h"
#include "TituEngine/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace TituEngine
{
	class TE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline int GetFrameCount() { return m_FrameCount; }

		inline static Application& Instance() { return *s_Instance; }
		ImGuiLayer* m_ImGuiLayer;

		void Close() { m_Runing = false; }

	private:
		void Run();
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		float m_CurrentTime;
		unsigned long int m_FrameCount;
		friend int ::main(int argc, char** argv);
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
