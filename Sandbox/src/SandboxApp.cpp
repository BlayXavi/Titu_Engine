#include <TituEngine.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "imgui/imgui.h"

using namespace TituEngine;

class SandboxLayer : public Layer
{
public:
	SandboxLayer() : Layer("SBLayer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f)
	{
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

	void OnImGuiRender() override
	{
	}

	void OnUpdate(Timestep ts) override
	{
		TE_CLIENT_TRACE("Delta time {0} ({1}ms)", ts.GetDeltaTime(), ts.GetDeltaTimeMilliseconds());

		if (InputBridge::IsKeyPressed(TE_KEY_UP))
			m_CameraPosition.y += m_CameraSpeed * ts;
		else if (InputBridge::IsKeyPressed(TE_KEY_DOWN))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (InputBridge::IsKeyPressed(TE_KEY_RIGHT))
			m_CameraPosition.x += m_CameraSpeed * ts;
		else if (InputBridge::IsKeyPressed(TE_KEY_LEFT))
			m_CameraPosition.x -= m_CameraSpeed * ts;

		if (InputBridge::IsKeyPressed(TE_KEY_Q))
			m_CameraRotation += m_CameraAngularSpeed * ts;
		else if (InputBridge::IsKeyPressed(TE_KEY_E))
			m_CameraRotation -= m_CameraAngularSpeed * ts;

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_Camera->SetPosition(m_CameraPosition);
		m_Camera->SetZRotation(m_CameraRotation);

		Renderer::BeginScene(m_Camera);
		{
			Renderer::Submit(m_VertexArray, m_Shader);
		}

		Renderer::EndScene();
	}

private:

	Shader* m_Shader = nullptr;
	VertexArray* m_VertexArray = nullptr;
	OrthographicCamera* m_Camera = nullptr;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed;

	float m_CameraAngularSpeed;
	float m_CameraRotation;
};

class Sandbox : public TituEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{

	}

};

TituEngine::Application* TituEngine::CreateApplication()
{
	return new Sandbox();
}
