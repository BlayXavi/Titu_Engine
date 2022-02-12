#include "tepch.h"
#include "Application.h"

#include "TituEngine/Input/Input.h"
#include "TituEngine/Renderer/Renderer.h"

namespace TituEngine
{
#define BIND_EVENT_FN(x) std::bind(&Application::##x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_CurrentTime(0.0f), m_FrameCount(0)
	{
		TE_PROFILE_PROFILE_FUNC();

		TE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		TE_PROFILE_PROFILE_FUNC();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrollEvent));

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::Run()
	{
		while (m_Runing)
		{
			TE_PROFILE_PROFILE_FUNC();

			if (m_Minimized == false)
			{
				TituEngine::Input::UpdateMouse();
				float time = TituEngine::Timestep::GetTime();
				Timestep ts = time - m_CurrentTime; //same as instancing a float!
				m_CurrentTime = time;

				for (Layer* layer : m_LayerStack) //compiler understand it because of implementation of begin() & end()
					layer->OnUpdate(ts);
			}

			{
				TE_PROFILE_PROFILE_SCOPE("ImGui Render");
				m_ImGuiLayer->BeginRender();
				for (Layer* layer : m_LayerStack) //compiler understand it because of implementation of begin() & end()
					layer->OnImGuiRender();
				m_ImGuiLayer->EndRender();
				
				TituEngine::Input::PostUpdateMouse();
			}


			m_Window->OnUpdate();
			m_FrameCount++;
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Runing = false;
		TE_CORE_INFO(e.ToString());
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return true;
		}

		m_Minimized = false;

		Renderer::OnWindowResized(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnMouseScrollEvent(MouseScrolledEvent& e)
	{
		Input::OnMouseScrollEvent(e);
		return false;
	}
}
