#include "tepch.h"
#include "Application.h"

#include "glad/glad.h"
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

		BufferLayout layout =
		{
			{ShaderDataType::Float3, false, "a_Position"},
			{ShaderDataType::Float4, false, "a_Color"}
		};

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		m_VBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		m_IBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));

		m_VBuffer->SetLayout(layout);

		uint index = 0;
		const BufferLayout currentLayout = m_VBuffer->GetLayout();
		for (const BufferElement& element : currentLayout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.ElementCount, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location  = 0) in vec3 a_Position;
			layout(location  = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;			

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Position = a_Position;
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;			

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, m_IBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
