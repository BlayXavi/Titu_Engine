#include "Sandbox2D.h"

#define QUADS_COUNT 50000

#define GENERATE_RANDOM(min, max) min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)))

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("SandBox 2D Layer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f),
	m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f), m_QuadColor(0.9f, 0.1f, 0.1f, 1.0f), m_QuadTextureColor(1.0f), m_zSquare(0.0f), m_squareRotationAccumulated(0.0f), m_BackgroundTileSize({ 10, 10 })
{
	m_TriangleTransform *= glm::scale(glm::mat4(1.0f), { 0.2f, 0.2f, 1.0f });

	m_Camera = new OrthographicCamera(1920.0f / 1080.0f);
	m_OrthographicCameraController.SetCamera(m_Camera);
	m_QuadTexture = Texture2D::Create("assets/textures2D/Checkerboard.png");
	m_QuadTexture2 = Texture2D::Create("assets/textures2D/blending_transparent_window.png");

	randomPositions = new glm::mat4[QUADS_COUNT];
	randomColors = new glm::vec4[QUADS_COUNT];

	std::srand((unsigned int)std::time(nullptr)); // use current time as seed for random generator
	for (size_t i = 0; i < QUADS_COUNT; i++)
	{
		float x = GENERATE_RANDOM(-1.0f, 1.0f);
		float y = GENERATE_RANDOM(-1.0f, 1.0f);

		float scale = GENERATE_RANDOM(0.1f, 0.2f);
		randomPositions[i] =
			glm::translate(glm::mat4(1.0f), { x, y, 0.0f }) *
			glm::rotate(glm::mat4(1.0f), GENERATE_RANDOM(-180.0f, 180.0f), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { scale, scale, scale });

		x = GENERATE_RANDOM(0.0f, 1.0f);
		y = GENERATE_RANDOM(0.0f, 1.0f);
		float z = GENERATE_RANDOM(0.0f, 1.0f);
		float a = GENERATE_RANDOM(0.5f, 1.0f);

		randomColors[i] = { x, y, z, a };
	}

	memset(debugFPS, 0, FPS_DEBUG_COUNT * sizeof(float));
	memset(debugMS, 0.0f, FPS_DEBUG_COUNT * sizeof(float));

	m_VSync = Application::Instance().GetWindow().IsVsync();
}

Sandbox2DLayer::~Sandbox2DLayer()
{
	delete m_Camera;
}

void Sandbox2DLayer::OnImGuiRender()
{
	TE_PROFILE_PROFILE_FUNC();

	ImGui::Begin("Sandbox Inspector");

	if (ImGui::Checkbox("Vsync", &m_VSync))
		Application::Instance().GetWindow().SetVSync(m_VSync);

	if (ImGui::TreeNode("Render Stats"))
	{
		// Plots can display overlay texts
		// (in this example, we will display an average value)
		{
			float ms = currentTimeStep.GetDeltaTimeMilliseconds();
			float fps = 1000 / currentTimeStep.GetDeltaTimeMilliseconds();

			ImGui::Checkbox("Animate FPS Graph", &m_UpdateFPS);

			if (m_UpdateFPS)
			{
				m_AverageFPS = 0;
				for (int n = 1; n < FPS_DEBUG_COUNT; n++)
				{
					debugFPS[n - 1] = debugFPS[n];
					m_AverageFPS += debugFPS[n];
				}

				debugFPS[FPS_DEBUG_COUNT - 1] = fps;
				m_AverageFPS += debugFPS[FPS_DEBUG_COUNT - 1];

				m_AverageFPS /= (float)FPS_DEBUG_COUNT;
			}

			char overlay[32];
			sprintf(overlay, "FPS Avg: %f", m_AverageFPS);
			ImGui::PlotHistogram("Histogram", debugFPS, FPS_DEBUG_COUNT, 0, overlay, 0.0f, m_AverageFPS * 1.5f, ImVec2(0, 80.0f));
			ImGui::Separator();

			ImGui::Text("Batches [%d]", Renderer2D::RenderStats::GetDrawCalls());
			ImGui::Text("Quads [%d]", Renderer2D::RenderStats::GetQuads());
			ImGui::Text("Vertices [%d]", Renderer2D::RenderStats::GetVertices());

			ImGui::TreePop();
		}
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
		int count = 0;
		for (size_t i = 0; i < QUADS_COUNT; i++)
		{
			if (i == 0)
			{
				Renderer2D::DrawQuad(m_TriangleTransform, randomColors[i]);
			}
			else
			{
				if (count == 0)
					Renderer2D::DrawQuad(randomPositions[i], randomColors[i]);
				else if (count == 1)
					Renderer2D::DrawQuad(randomPositions[i], { 1.0f, 1.0f, 1.0f, 1.0f }, m_QuadTexture, { 1.0f, 1.0f });
				else if (count == 2)
					Renderer2D::DrawQuad(randomPositions[i], { 1.0f, 1.0f, 1.0f, 1.0f }, m_QuadTexture2, { 2.0f, 2.0f });
			}

			count++;
			if (count > 2)
				count = 0;
		}

		//Renderer2D::DrawQuad({ 0.0f, 0.0f, -9.99f }, { 20.0f, 20.0f }, m_QuadTextureColor, *m_QuadTexture, m_BackgroundTileSize);
		//Renderer2D::DrawQuad({ -1.0f, 0.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, { 0.3f, 0.3f, 0.9f, 1.0f }); //rotate quad


		//Renderer2D::DrawQuad({ 0.5f, 0.5f, m_zSquare }, { 0.5f, 1.0f }, glm::vec4(0.2f, 0.2f, 0.8f, 0.8f));//translucent blue quad
		//Renderer2D::DrawQuad({ 1.0f, 0.0f , 1.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, m_QuadTextureColor, *m_QuadTexture2); //window
	}

	Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
