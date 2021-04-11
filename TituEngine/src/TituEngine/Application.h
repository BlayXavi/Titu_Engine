#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Layer/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "TituEngine/ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

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
		bool OnWindowsClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Runing = true;

		LayerStack m_LayerStack;
		static Application* s_Instance;

		//TEMP
		unsigned int vertexArray = 0;
		Shader* m_Shader = nullptr;
		VertexBuffer* m_VBuffer = nullptr;
		IndexBuffer* m_IBuffer = nullptr;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}
