#include "Sandbox2D.h"
#include <iomanip>

#define QUADS_COUNT 50000

Sandbox2DLayer::Sandbox2DLayer()
	: Layer("SandBox 2D Layer"), m_CameraPosition(glm::vec3(0.0f)), m_CameraSpeed(1.0f), m_CameraRotation(0.0f), m_CameraAngularSpeed(45.0f),
	m_TriangleSpeed(1.0f), m_TriangleTransform(1.0f), m_TriangleAngularSpeed(1.0f), m_QuadColor(0.9f, 0.1f, 0.1f, 1.0f), m_QuadTextureColor(1.0f), m_zSquare(0.0f), m_squareRotationAccumulated(0.0f), m_BackgroundTileSize({ 10, 10 })
{
	m_ParticleSystem = ParticleSystem(99999);

	m_particleVel = { -1.0f, 1.0f, 0.0f, 1.0f };
	m_particleSizeStart = 0.3f;
	m_particleSizeEnd = 0.0f;
	m_particleColorStart = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_particleColorEnd = { 0.2f, 1.0f, 1.0f, 0.3f };
	m_particleAngularVel = 1.0f;
	m_particleLifeTime = 1.0f;
	m_particlesPerFrame = 1;

	m_TriangleTransform *= glm::scale(glm::mat4(1.0f), { 0.2f, 0.2f, 1.0f });

	FramebufferSpecs spec;
	spec.Width = 1280;
	spec.Height = 720;
	m_Framebuffer = Framebuffer::Create(spec);

	m_Camera = new OrthographicCamera(1280.0f / 720.0f);
	m_OrthographicCameraController.SetCamera(m_Camera);
	m_QuadTexture = Texture2D::Create("assets/textures2D/Checkerboard.png");
	m_QuadTexture2 = Texture2D::Create("assets/textures2D/blending_transparent_window.png");
	m_SpriteSheet = Texture2D::Create("assets/textures2D/base_Spritesheet.png");

	m_SubTexture2D = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });

	m_SubTextures2D[0] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[1] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[2] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[3] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[4] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[5] = new SubTexture2D(m_SpriteSheet, { 0, 3 }, { 128, 128 }, { 1, 1 });
	m_SubTextures2D[6] = new SubTexture2D(m_SpriteSheet, { 0, 1 }, { 128, 128 }, { 1, 2 });
	m_SubTextures2D[7] = new SubTexture2D(m_SpriteSheet, { 1, 1 }, { 128, 128 }, { 1, 2 });
	m_SubTextures2D[8] = new SubTexture2D(m_SpriteSheet, { 3, 1 }, { 128, 128 }, { 1, 2 });
	m_SubTextures2D[9] = new SubTexture2D(m_SpriteSheet, { 4, 1 }, { 128, 128 }, { 1, 2 });

	randomPositions = new glm::mat4[QUADS_COUNT];
	randomColors = new glm::vec4[QUADS_COUNT];

	std::srand((unsigned int)std::time(nullptr)); // use current time as seed for random generator
	for (size_t i = 0; i < QUADS_COUNT; i++)
	{
		float x = GENERATE_RANDOM(-4.0f, 4.0f);
		float y = GENERATE_RANDOM(-4.0f, 4.0f);

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
	memset(debugMS, 0, FPS_DEBUG_COUNT * sizeof(float));

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
				m_AverageMS = 0;
				for (int n = 1; n < FPS_DEBUG_COUNT; n++)
				{
					debugFPS[n - 1] = debugFPS[n];
					debugMS[n - 1] = debugMS[n];
					m_AverageFPS += debugFPS[n];
					m_AverageMS += debugMS[n];

				}

				debugFPS[FPS_DEBUG_COUNT - 1] = fps;
				debugMS[FPS_DEBUG_COUNT - 1] = ms;
				m_AverageFPS += debugFPS[FPS_DEBUG_COUNT - 1];
				m_AverageMS += debugMS[FPS_DEBUG_COUNT - 1];

				m_AverageFPS /= (float)FPS_DEBUG_COUNT;
				m_AverageMS /= (float)FPS_DEBUG_COUNT;
			}

			char overlay[32];
			sprintf_s(overlay, "FPS Avg: %f", m_AverageFPS);
			ImGui::PlotHistogram("FPS Histogram", debugFPS, FPS_DEBUG_COUNT, 0, overlay, 0.0f, m_AverageFPS * 1.5f, ImVec2(0, 80.0f));
			ImGui::Separator();
			sprintf_s(overlay, "MS Avg: %f", m_AverageMS);
			ImGui::PlotHistogram("ms Histogram", debugMS, FPS_DEBUG_COUNT, 0, overlay, 0.0f, m_AverageMS * 1.5f, ImVec2(0, 80.0f));

			ImGui::Text("Batches [%d]", Renderer2D::RenderStats::GetDrawCalls());
			ImGui::Text("Quads [%d]", Renderer2D::RenderStats::GetQuads());
			ImGui::Text("Vertices [%d]", Renderer2D::RenderStats::GetVertices());

			ImGui::TreePop();
		}
	}

	if (ImGui::TreeNode("Particle Props"))
	{
		ImGui::SliderInt("Particles per Frame", &m_particlesPerFrame, 1, 200);
		ImGui::SliderFloat("LifeTime", &m_particleLifeTime, 0.0f, 30.0f);
		ImGui::SliderFloat4("Velocity", &m_particleVel[0], -5.0f, 5.0f);
		ImGui::SliderFloat("Size Start", &m_particleSizeStart, 0.0f, 3.0f);
		ImGui::SliderFloat("Size End", &m_particleSizeEnd, 0.0f, 3.0f);
		ImGui::ColorEdit4("Color Start", &m_particleColorStart[0]);
		ImGui::ColorEdit4("Color End", &m_particleColorEnd[0]);
		ImGui::SliderFloat("Angualr Speed", &m_particleAngularVel, 0.0f, 5.0f);
		ImGui::TreePop();
	}

	uint32_t framebufferTextureID = m_Framebuffer->GetColorAttachment();
	ImGui::Image((void*)framebufferTextureID, ImVec2{ 512.0f, 256.0f }, { 0, 1 }, { 1, 0 });

	ImGui::End();
	ImGui::ShowDemoWindow();
}

void Sandbox2DLayer::OnUpdate(Timestep ts)
{
	/*float ms = currentTimeStep.GetDeltaTimeMilliseconds();
	float fps = 1000 / currentTimeStep.GetDeltaTimeMilliseconds();
	std::cout << std::fixed;
	std::cout << "Ms: ";
	std::cout << std::setprecision(2);
	std::cout << ms << " FPS ";
	std::cout << (int)fps << std::endl;*/
	TE_PROFILE_PROFILE_FUNC();

	{
		m_OrthographicCameraController.OnUpdate(ts);
	}

	currentTimeStep = ts;

	if (InputBridge::IsButtonMousePressed(TE_MOUSE_BUTTON_1))
	{
		for (size_t i = 0; i < m_particlesPerFrame; i++)
		{
			ParticleProperties pps;
			pps.velocity = { GENERATE_RANDOM(m_particleVel.x, m_particleVel.y), GENERATE_RANDOM(m_particleVel.z, m_particleVel.w) };
			pps.colorStart = m_particleColorStart;
			pps.colorEnd = m_particleColorEnd;
			pps.sizeStart = { m_particleSizeStart, m_particleSizeStart };
			pps.sizeEnd = { m_particleSizeEnd, m_particleSizeEnd };
			pps.rotationSpeed = m_particleAngularVel;
			pps.lifeTime = m_particleLifeTime;

			std::pair<float, float> mousePos = InputBridge::GetMousePosition();
			glm::vec3 tempPos = m_Camera->ScreenSpacePosToWorldPos(mousePos.first, mousePos.second);

			pps.posStart = { tempPos.x, tempPos.y };

			m_ParticleSystem.Emit(pps);
		}
	}

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

	m_ParticleSystem.OnUpdate(ts);

	m_Framebuffer->Bind();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera);
	{
		TE_PROFILE_PROFILE_SCOPE("Sandbox2DLayer::BeginDraw");
		/*int count = 0;
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
		}*/

		RenderCommand::EnableDepthTest(false);
		m_ParticleSystem.OnRender();
		RenderCommand::EnableDepthTest(true);

		//Renderer2D::DrawQuad(glm::mat4(1.0), { 1.0f, 1.0f, 1.0f, 1.0f }, m_SubTexture2D, { 1.0f, 1.0f });
		//Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0), { 0.2f, 0.0f, 1.0f }), { 1.0f, 1.0f, 1.0f, 1.0f }, m_SubTexture2D, { 1.0f, 1.0f });
		Renderer2D::DrawQuad({ -1.0f, 0.0f, 1.0f }, m_SubTextures2D[9]->GetSpriteSize(), { 1.0f, 1.0f, 1.0f, 1.0f }, m_SubTextures2D[9], { 1.0f, 1.0f });

		//Renderer2D::DrawQuad({ 0.0f, 0.0f, -9.99f }, { 20.0f, 20.0f }, m_QuadTextureColor, *m_QuadTexture, m_BackgroundTileSize);
		//Renderer2D::DrawQuad({ -1.0f, 0.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, { 0.3f, 0.3f, 0.9f, 1.0f }); //rotate quad


		//Renderer2D::DrawQuad({ 0.5f, 0.5f, m_zSquare }, { 0.5f, 1.0f }, glm::vec4(0.2f, 0.2f, 0.8f, 0.8f));//translucent blue quad
		//Renderer2D::DrawQuad({ 1.0f, 0.0f , 1.0f }, glm::radians(m_squareRotationAccumulated), { 1.0f, 1.0f }, m_QuadTextureColor, *m_QuadTexture2); //window
	}

	Renderer2D::EndScene();
	m_Framebuffer->UnBind();
}

void Sandbox2DLayer::OnEvent(Event& e)
{
	m_OrthographicCameraController.OnEvent(e);
}
