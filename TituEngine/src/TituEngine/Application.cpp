#include "tepch.h"
#include "Application.h"

#include "Input/Input.h"

namespace TituEngine
{
#define BIND_EVENT_FN(x) std::bind(&Application::##x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		TE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr< Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		float vertices[3 * 3 + 3 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.9f, 1.0f,
			-0.0f, 0.5f, 0.0f, 0.0f, 0.9f, 0.9f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.2f, 1.0f
		};

		uint indices[3] = { 0, 1, 2 };
		m_VertexArray = VertexArray::Create();

		BufferLayout layout =
		{
			{ShaderDataType::Float3, false, "a_Position"},
			{ShaderDataType::Float4, false, "a_Color"}
		};

		VertexBuffer* m_VBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		IndexBuffer* m_IBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
		m_Camera = new OrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);

		m_VBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VBuffer);
		m_VertexArray->SetIndexBuffer(m_IBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location  = 0) in vec3 a_Position;
			layout(location  = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;			

			uniform mat4 u_ViewProjectionMatrix;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;			

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader = new Shader(vertexSrc, fragmentSrc);

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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			m_Camera->SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera->SetZRotation(45.0f);

			Renderer::BeginScene(m_Camera);
			{
				Renderer::Submit(m_VertexArray, m_Shader);
			}

			for (Layer* layer : m_LayerStack) //compiler understand it because of implementation of begin() & end()
				layer->OnUpdate();

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
