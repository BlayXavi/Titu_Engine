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
	SandboxLayer() : Layer("SBLayer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f), m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f)
	{
		float vertices[3 * 4/*vertices*/ + 2 * 4/*TextCoords*/] =
		{
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f
		};

		uint indices[3 * 2] = { 0, 1, 3, 1, 2, 3 };
		m_VertexArray = VertexArray::Create();

		BufferLayout layout =
		{
			{ShaderDataType::Float3, false, "a_Position"},
			{ShaderDataType::Float2, false, "a_TextCoord"},
			//{ShaderDataType::Float4, false, "a_Color"}
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
			layout(location = 1) in vec2 a_TextCoord;
			//layout(location  = 1) in vec4 a_Color;

			out vec3 v_Position;
			//out vec4 v_Color;
			out vec2 v_TextCoord;

			uniform mat4 u_ModelViewProjectionMatrix;

			void main()
			{
				v_Position = a_Position;
				//v_Color = a_Color;
				v_TextCoord = a_TextCoord;
				gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			out vec4 color;
			in vec3 v_Position;
			//in vec4 v_Color;			
			in vec2 v_TextCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TextCoord);
			}
		)";

		texture = Texture2D::Create("assets/textures2D/Checkerboard.png");
		m_BlendTexture = Texture2D::Create("assets/textures2D/grass.png");
		m_Shader = new Shader(vertexSrc, fragmentSrc);
		m_Shader->UploadUniformInt("u_Texture", 0);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Sandbox Inspector");
		ImGui::DragFloat("Triangle Speed", &m_TriangleSpeed, 0.2f, 0.0f, 10.0f);
		ImGui::DragFloat("Triangle Angular Speed", &m_TriangleAngularSpeed, 0.2f, 0.0f, 10.0f);
		ImGui::End();
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

		if (InputBridge::IsKeyPressed(TE_KEY_D))
			m_TriangleTransform = glm::translate(m_TriangleTransform, glm::vec3(m_TriangleSpeed * ts, 0.0f, 0.0f));
		if (InputBridge::IsKeyPressed(TE_KEY_A))
			m_TriangleTransform = glm::translate(m_TriangleTransform, glm::vec3(-m_TriangleSpeed * ts, 0.0f, 0.0f));
		if (InputBridge::IsKeyPressed(TE_KEY_W))
			m_TriangleTransform = glm::translate(m_TriangleTransform, glm::vec3(0.0f, m_TriangleSpeed * ts, 0.0f));
		if (InputBridge::IsKeyPressed(TE_KEY_S))
			m_TriangleTransform = glm::translate(m_TriangleTransform, glm::vec3(0.0f, -m_TriangleSpeed * ts, 0.0f));

		if (InputBridge::IsKeyPressed(TE_KEY_Z))
			m_TriangleTransform = glm::rotate(m_TriangleTransform, m_TriangleAngularSpeed * ts, glm::vec3(0.0f, 0.0f, 1.0f));
		if (InputBridge::IsKeyPressed(TE_KEY_X))
			m_TriangleTransform = glm::rotate(m_TriangleTransform, -m_TriangleAngularSpeed * ts, glm::vec3(0.0f, 0.0f, 1.0f));

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
			texture->Bind();
			Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0));
			m_BlendTexture->Bind();
			Renderer::Submit(m_VertexArray, m_Shader, m_TriangleTransform);
		}

		Renderer::EndScene();
	}

private:

	Shader* m_Shader = nullptr;
	VertexArray* m_VertexArray = nullptr;
	OrthographicCamera* m_Camera = nullptr;
	Texture* texture = nullptr;
	Texture* m_BlendTexture = nullptr;

	glm::mat4 m_TriangleTransform;
	float m_TriangleSpeed;
	float m_TriangleAngularSpeed;

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
