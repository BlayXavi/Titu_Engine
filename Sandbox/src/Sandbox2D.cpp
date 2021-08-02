#include "Sandbox2D.h"

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("SandBox 2D Layer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f),
	m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f), m_QuadColor(0.9f, 0.1f, 0.1f, 1.0f), m_QuadTextureColor(1.0f), m_zSquare(0.0f), m_squareRotationAccumulated(0.0f), m_BackgroundTileSize({ 10, 10 })
{
	m_Camera = new OrthographicCamera(1920.0f / 1080.0f);
	m_OrthographicCameraController.SetCamera(m_Camera);
	m_QuadTexture = Texture2D::Create("assets/textures2D/Checkerboard.png");
	m_QuadTexture2 = Texture2D::Create("assets/textures2D/blending_transparent_window.png");
}

Sandbox2DLayer::~Sandbox2DLayer()
{
	delete m_Camera;
}

void Sandbox2DLayer::OnImGuiRender()
{
	TE_PROFILE_PROFILE_FUNC();

	ImGui::Begin("Sandbox Inspector");

	ImGui::Text("Delta time %f (%fms)", currentTimeStep.GetDeltaTime(), currentTimeStep.GetDeltaTimeMilliseconds());
	ImGui::SliderFloat("Triangle Speed", &m_TriangleSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat("Triangle Angular Speed", &m_TriangleAngularSpeed, 0.0f, 10.0f);
	ImGui::SliderFloat("z background", &m_zSquare, -1.0f, 1.0f);
	ImGui::SliderInt2("Background Tile Size", &m_BackgroundTileSize[0], 1, 10);
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
	TE_PROFILE_PROFILE_FUNC();

	{
		m_OrthographicCameraController.OnUpdate(ts);
	}

	currentTimeStep = ts;

	{
		TE_PROFILE_PROFILE_SCOPE("Sandbox2DLayer::ProcessInput");
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

		m_squareRotationAccumulated += 180.0f * ts;
		if (m_squareRotationAccumulated > 360.0f)
			m_squareRotationAccumulated -= 360.0f;
	}

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera);
	{
		TE_PROFILE_PROFILE_SCOPE("Sandbox2DLayer::BeginDraw");

		Renderer2D::DrawQuad({ 0.0f, 0.0f, -9.99f }, { 20.0f, 20.0f }, m_QuadTextureColor, *m_QuadTexture, m_BackgroundTileSize);
		Renderer2D::DrawQuad(m_TriangleTransform, m_QuadColor); //movement quad
		Renderer2D::DrawQuad({ -1.0f, 0.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, { 0.3f, 0.3f, 0.9f, 1.0f }); //rotate quad


		Renderer2D::DrawQuad({ 0.5f, 0.5f, m_zSquare }, { 0.5f, 1.0f }, glm::vec4(0.2f, 0.2f, 0.8f, 0.8f));//translucent blue quad
		Renderer2D::DrawQuad({ 1.0f, 0.0f , 1.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, m_QuadTextureColor, *m_QuadTexture2); //window
	}

	Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
