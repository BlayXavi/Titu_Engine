#include "Sandbox2D.h"

Sandbox2DLayer::Sandbox2DLayer() 
	: Layer("SandBox 2D Layer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f), m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f)
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
	};

	VertexBuffer* m_VBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
	IndexBuffer* m_IBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint));
	m_Camera = new OrthographicCamera(1920.0f / 1080.0f);
	m_OrthographicCameraController.SetCamera(m_Camera);

	m_VBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(m_VBuffer);
	m_VertexArray->SetIndexBuffer(m_IBuffer);

	texture = Texture2D::Create("assets/textures2D/Checkerboard.png");
	m_BlendTexture = Texture2D::Create("assets/textures2D/grass.png");
	m_Shader = Shader::Create("assets/shaders/testing/Blending.glsl");

	if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
		static_cast<TituEngine::OpenGLShader*>(m_Shader)->UploadUniformInt("u_Texture", 0);
}

Sandbox2DLayer::~Sandbox2DLayer()
{
	delete m_Shader;
	delete m_VertexArray;
	delete m_Camera;
	delete texture;
	delete m_BlendTexture;
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Sandbox Inspector");

	ImGui::Text("Delta time %f (%fms)", currentTimeStep.GetDeltaTime(), currentTimeStep.GetDeltaTimeMilliseconds());
	ImGui::DragFloat("Triangle Speed", &m_TriangleSpeed, 0.2f, 0.0f, 10.0f);
	ImGui::DragFloat("Triangle Angular Speed", &m_TriangleAngularSpeed, 0.2f, 0.0f, 10.0f);
	if (ImGui::Button("Reset"))
	{
		m_TriangleSpeed = 1.0f;
		m_TriangleAngularSpeed = 1.0f;
	}
	ImGui::End();
}

void Sandbox2DLayer::OnUpdate(Timestep ts)
{
	m_OrthographicCameraController.OnUpdate(ts);

	currentTimeStep = ts;

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

	Renderer::BeginScene(m_Camera);
	{
		texture->Bind();
		Renderer::Submit(m_VertexArray, m_Shader, glm::mat4(1.0));
		m_BlendTexture->Bind();
		Renderer::Submit(m_VertexArray, m_Shader, m_TriangleTransform);
	}

	Renderer::EndScene();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
