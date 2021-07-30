#include "Sandbox2D.h"

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("SandBox 2D Layer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f),
	m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f), m_QuadColor(0.9f, 0.1f, 0.1f, 1.0f), m_QuadTextureColor(1.0f), m_zSquare(0.0f)
{
	m_Camera = new OrthographicCamera(1920.0f / 1080.0f);
	m_OrthographicCameraController.SetCamera(m_Camera);
	m_QuadTexture = Texture2D::Create("assets/textures2D/Checkerboard.png");
}

Sandbox2DLayer::~Sandbox2DLayer()
{
	delete m_Camera;
}

void Sandbox2DLayer::OnImGuiRender()
{
	ImGui::Begin("Sandbox Inspector");

	ImGui::Text("Delta time %f (%fms)", currentTimeStep.GetDeltaTime(), currentTimeStep.GetDeltaTimeMilliseconds());
	ImGui::DragFloat("Triangle Speed", &m_TriangleSpeed, 0.2f, 0.0f, 10.0f);
	ImGui::DragFloat("Triangle Angular Speed", &m_TriangleAngularSpeed, 0.2f, 0.0f, 10.0f);
	ImGui::DragFloat("z background", &m_zSquare, 0.2f, -1.0f, 1.0f);
	ImGui::ColorEdit4("Square Color", &m_QuadColor[0]);
	ImGui::ColorEdit4("Texture Color", &m_QuadTextureColor[0]);

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

	Renderer2D::BeginScene(m_Camera);
	{
		Renderer2D::DrawQuad({ 0.0f, 0.0f, -9.99f}, { 20.0f, 20.0f }, m_QuadTextureColor, *m_QuadTexture);
		Renderer2D::DrawQuad(m_TriangleTransform, m_QuadColor);
		Renderer2D::DrawQuad({ 0.5f, 0.5f, m_zSquare }, { 0.5f, 1.0f }, glm::vec4(0.2f, 0.2f, 0.8f, 0.8f));
	}

	Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
