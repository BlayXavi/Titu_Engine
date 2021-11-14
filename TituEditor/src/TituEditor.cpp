#include "TituEditor.h"
#include <iomanip>

#include "TituEngine/Scene/SceneSerializer.h"

#include "Editor/Utils/WindowDialogs.h"

#define QUADS_COUNT 50000

namespace TituEngine
{
	class CameraController : public NativeScript
	{
	public:
		bool Active = false;

		void OnCreate() override
		{
		}

		void OnUpdate(Timestep ts) override
		{
			if (!Active)
				return;

			TransformComponent& tc = GetComponent<TransformComponent>();
			glm::vec3 pos = tc.GetTranslation();

			float speed = 5.0f;

			if (Input::IsKeyPressed(TE_KEY_A))
				pos.x -= speed * ts;
			if (Input::IsKeyPressed(TE_KEY_D))
				pos.x += speed * ts;
			if (Input::IsKeyPressed(TE_KEY_W))
				pos.y += speed * ts;
			if (Input::IsKeyPressed(TE_KEY_S))
				pos.y -= speed * ts;
			if (Input::IsKeyPressed(TE_KEY_Q))
				pos.z += speed * ts;
			if (Input::IsKeyPressed(TE_KEY_E))
				pos.z -= speed * ts;

			pos.z -= Input::GetScrollDelta() * speed * ts;

			tc.SetTranslation(pos);
		}
	};


	TituEditorLayer::TituEditorLayer()
		: Layer("TituEditor Layer")
	{

		FramebufferSpecs fbSpecs;
		fbSpecs.Width = 1280;
		fbSpecs.Height = 720;
		fbSpecs.Samples = 1;
		m_Framebuffer = Framebuffer::Create(fbSpecs);

		m_EditorCamera = new TransformedCamera();
		m_EditorCamera->SetProjectionType(Camera::Projection::ORTHOGRAPHIC);
		m_EditorCamera->SetViewportSize(fbSpecs.Width, fbSpecs.Height);
		m_EditorCamera->SetPosition(m_EditorCamera->GetPosition() + glm::vec3(0.0f, 0.0f, 3.0f));
		m_EditorCamera->SetNearPlane(-10.0f);
		m_EditorCamera->SetFarPlane(10.0f);
		m_EditorCamera->SetProjectionType(Camera::Projection::ORTHOGRAPHIC);
		m_EditorCamera->SetViewportSize(fbSpecs.Width, fbSpecs.Height);
		m_EditorCamera->SetAsActiveCamera();
		m_CameraController = new EditorOrthographicCameraController(m_EditorCamera);

		m_Scene = new Scene();
		SceneSerializer::DeserializeScene(m_Scene, "assets\\scene\\basic_Scene.tituscene");

#if 0 
		Entity entity = m_Scene->CreateEntity("Entity1");
		TransformComponent tc = entity.AddComponent<TransformComponent>();
		tempSpriteRendererComponent = &entity.AddComponent<SpriteRendererComponent>();
		Entity entity2 = m_Scene->CreateEntity("Entity2");
		TransformComponent tc2 = entity2.AddComponent<TransformComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
		&entity2.AddComponent<SpriteRendererComponent>(glm::vec4(0.6f, 0.8f, 0.2f, 1.0f));

		m_GameCameraEntity = m_Scene->CreateEntity("Camera_Entity");
		TransformComponent& t = m_GameCameraEntity.AddComponent<TransformComponent>();
		t.SetTranslation(t.GetTranslation() + glm::vec3(0.0f, 0.0f, 3.0f));
		m_GameCamera = &m_GameCameraEntity.AddComponent<CameraComponent>().Camera;


		NativeScriptComponent& nsc = m_GameCameraEntity.AddComponent<NativeScriptComponent>();
		nsc.Bind<CameraController>();
		nsc.Instance = nsc.InstantiateScript();
		nsc.Instance->m_Entity = m_GameCameraEntity;
#endif

		memset(debugFPS, 0, FPS_DEBUG_COUNT * sizeof(float));
		memset(debugMS, 0, FPS_DEBUG_COUNT * sizeof(float));

		m_VSync = Application::Instance().GetWindow().IsVsync();

		m_SpriteSheet = Texture2D::Create("assets/textures2D/base_Spritesheet.png");

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
	}

	TituEditorLayer::~TituEditorLayer()
	{
		delete m_CameraController;
		delete m_EditorCamera;
		delete m_SpriteSheet;
		delete m_Framebuffer;
		for (size_t i = 0; i < 10; i++)
			delete m_SubTextures2D[i];
		delete m_Scene;
	}

	void TituEditorLayer::OnImGuiRender()
	{
		TE_PROFILE_PROFILE_FUNC();

		{
			static bool show_rendererStats = true;
			static bool show_MouseStats = false;
			static bool show_EditorCamera = false;
			static bool show_ImGuiDemo = false;
			//Dockspace Init
			{
				static bool dockspaceOpen = true;
				static bool opt_fullscreen_persistant = true;
				bool opt_fullscreen = opt_fullscreen_persistant;
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
				if (opt_fullscreen)
				{
					ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->Pos);
					ImGui::SetNextWindowSize(viewport->Size);
					ImGui::SetNextWindowViewport(viewport->ID);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
					window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
					window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
				}

				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
				// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
				// all active windows docked into it will lose their parent and become undocked.
				// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
				// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
				ImGui::PopStyleVar();

				if (opt_fullscreen)
					ImGui::PopStyleVar(2);

				// DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}
			}

			//Tool Bar
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+S"))
					{
						SaveScene();
					}

					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Exit", "Alt+F4")) Application::Instance().Close();
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("TituEngine"))
				{
					ImGui::MenuItem("Scene Hierarchy", NULL, &SceneHierarchyPanel::m_OpenSceneHierarchy);
					ImGui::MenuItem("Renderer stats", NULL, &show_rendererStats);
					ImGui::MenuItem("Mouse stats", NULL, &show_MouseStats);
					ImGui::MenuItem("Editor Camera Setigns", NULL, &show_EditorCamera);



					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Helpers"))
				{
					ImGui::MenuItem("Show ImGui DemoWindow", NULL, &show_ImGuiDemo);
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_SceneHierarchyPanel.OnImGuiRender(m_Scene);

			//Sandbox Inspector
			{
				if (show_rendererStats)
				{
					ImGui::Begin("Render Stats", &show_rendererStats);

					if (ImGui::Checkbox("Vsync", &m_VSync))
						Application::Instance().GetWindow().SetVSync(m_VSync);

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
						ImGui::PlotHistogram("FPS", debugFPS, FPS_DEBUG_COUNT, 0, overlay, 0.0f, m_AverageFPS * 1.5f, ImVec2(0, 80.0f));
						ImGui::Separator();
						sprintf_s(overlay, "MS Avg: %f", m_AverageMS);
						ImGui::PlotHistogram("ms", debugMS, FPS_DEBUG_COUNT, 0, overlay, 0.0f, m_AverageMS * 1.5f, ImVec2(0, 80.0f));

						ImGui::Text("Batches [%d]", Renderer2D::RenderStats::GetDrawCalls());
						ImGui::Text("Quads [%d]", Renderer2D::RenderStats::GetQuads());
						ImGui::Text("Vertices [%d]", Renderer2D::RenderStats::GetVertices());


					}

					ImGui::End(); //Render Stats
				}

				if (show_MouseStats)
				{
					ImGui::Begin("Mouse Stats", &show_MouseStats);

					std::pair<float, float> mousePos = Input::GetMousePosition();
					std::string xy = std::to_string((int)mousePos.first) + ", " + std::to_string((int)mousePos.second);
					ImGui::LabelText(xy.c_str(), "MousePos: ", "");

					std::pair<float, float> mousePosDelta = Input::GetMouseDeltaPosition();
					xy = std::to_string((int)mousePosDelta.first) + ", " + std::to_string((int)mousePosDelta.second);
					ImGui::LabelText(xy.c_str(), "MouseDelta: ", "");

					glm::mat4 viewProjectionMatrix = Camera::GetActiveCamera().GetViewProjectionMatrix();
					glm::vec2 mousePosWorld = Camera::GetActiveCamera().GetCamera()->ScreenSpacePosToWorldPos(mousePos.first, mousePos.second, viewProjectionMatrix);
					std::string xyWorld = std::to_string(mousePosWorld.x) + ", " + std::to_string(mousePosWorld.y);
					ImGui::LabelText(xyWorld.c_str(), "MousePosWorld: ", "");

					ImGui::End(); //Mouse Stats
				}

				if (show_EditorCamera)
				{
					ImGui::Begin("Editor Camera", &show_EditorCamera);

					ComponentPanelDrawer::DrawCamera(*m_EditorCamera);
					Camera::ActiveCameraData actCamData = Camera::GetActiveCamera();
					if (actCamData.GetCamera() != m_EditorCamera && ImGui::Button("Retreive camera control"))
					{
						m_EditorCamera->SetAsActiveCamera();
					}
					ImGui::End();
				}
			}

			//Viewport
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
				ImGui::Begin("Viewport");

				m_ViewPortFocused = ImGui::IsWindowFocused();
				m_ViewPortHovered = ImGui::IsWindowHovered();
				Application::Instance().GetImGuiLayer()->SetBlockEvents(!m_ViewPortFocused || !m_ViewPortHovered);

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				if (m_ViewPortPanelSize != *(glm::vec2*)&viewportPanelSize)
				{
					m_ViewPortPanelSize = { viewportPanelSize.x, viewportPanelSize.y };
					m_Framebuffer->Resize((uint32_t)m_ViewPortPanelSize.x, (uint32_t)m_ViewPortPanelSize.y);

					//m_CameraController->OnResize((uint32_t)m_ViewPortPanelSize.x, (uint32_t)m_ViewPortPanelSize.y);
					Camera::GetActiveCamera().GetCamera()->SetViewportSize((uint32_t)m_ViewPortPanelSize.x, (uint32_t)m_ViewPortPanelSize.y);
					//m_GameCamera->SetViewportSize((uint32_t)m_ViewPortPanelSize.x, (uint32_t)m_ViewPortPanelSize.y);
				}

				uint64_t textureID = (uint64_t)m_Framebuffer->GetColorAttachment();
				ImGui::Image((void*)textureID, { m_ViewPortPanelSize.x,  m_ViewPortPanelSize.y }, { 0, 1 }, { 1, 0 });
				ImGui::End(); //viewport
				ImGui::PopStyleVar();
			}

			if (show_ImGuiDemo)
				ImGui::ShowDemoWindow(&show_ImGuiDemo);

			ImGui::End(); //dockspace 
		}
	}

	void TituEditorLayer::OnUpdate(Timestep ts)
	{
		TE_PROFILE_PROFILE_FUNC();

		/*float ms = currentTimeStep.GetDeltaTimeMilliseconds();
		float fps = 1000 / currentTimeStep.GetDeltaTimeMilliseconds();
		std::cout << std::fixed;
		std::cout << "Ms: ";
		std::cout << std::setprecision(2);
		std::cout << ms << " FPS ";
		std::cout << (int)fps << std::endl;*/

		currentTimeStep = ts;

		if (m_ViewPortFocused)
			m_CameraController->OnUpdate(ts);

		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Camera::ActiveCameraData activeCamera = Camera::GetActiveCamera();
		glm::mat4 viewProjectionMatrix = activeCamera.GetViewProjectionMatrix();

		Renderer2D::BeginScene(activeCamera.GetCamera(), viewProjectionMatrix);
		{
			TE_PROFILE_PROFILE_SCOPE("Sandbox2DLayer::BeginDraw");
			m_Scene->OnUpdate(ts);
		}

		Renderer2D::EndScene();
		m_Framebuffer->UnBind();
	}

	void TituEditorLayer::OnEvent(Event& e)
	{
		TE_PROFILE_PROFILE_FUNC();
		m_CameraController->OnEvent(e);
		EventDispatcher eDispatcher(e);
		eDispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent kpe)->bool {return this->OnKeyPressed(kpe); });
	}

	bool TituEditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;
		bool control = Input::IsKeyPressed(TE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(TE_KEY_RIGHT_CONTROL);
		bool alt = Input::IsKeyPressed(TE_KEY_LEFT_ALT) || Input::IsKeyPressed(TE_KEY_RIGHT_ALT);

		switch (e.GetKeyCode())
		{
		case TE_KEY_N:
			if (control)
				NewScene();
			break;
		case TE_KEY_O:
			if (control)
				OpenScene();
			break;
		case TE_KEY_S:
			if (control)
				SaveScene();
		}

		return false;
	}
	void TituEditorLayer::NewScene()
	{
		delete m_Scene;
		m_Scene = new Scene();
	}

	void TituEditorLayer::OpenScene()
	{
		std::string fName = FileDialogs::OpenFile("Titu Scene (*.tituscene)\0*.tituscene\0");
		if (!fName.empty())
		{
			delete m_Scene;
			m_Scene = new Scene();
			SceneSerializer::DeserializeScene(m_Scene, fName);
		}
	}

	void TituEditorLayer::SaveScene()
	{
		std::string fName = FileDialogs::SaveFile("Titu Scene (*.tituscene)\0*.tituscene\0");
		if (!fName.empty())
			SceneSerializer::SerializeScene(m_Scene, fName);
	}
}