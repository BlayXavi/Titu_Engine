#pragma once

#include <TituEngine.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include "imgui/imgui.h"

#include "TituEngine/Instrumentation/Profiler.h"

#include "Controller/EditorOrthographicCameraController.h"

#include "Editor/SceneHierarchyPanel.h"
#include "Editor/ComponentPanelDrawer.h"

#define FPS_DEBUG_COUNT 120

namespace TituEngine
{
	class TituEditorLayer : public Layer
	{
	public:
		TituEditorLayer();
		~TituEditorLayer();

		void OnImGuiRender() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;


	private:

		SceneHierarchyPanel m_SceneHierarchyPanel;

		Camera* m_ActiveCamera = nullptr;

		Scene* m_Scene = nullptr;
		SpriteRendererComponent* tempSpriteRendererComponent;

		Timestep currentTimeStep;

		EditorOrthographicCameraController* m_CameraController = nullptr;
		TransformedCamera* m_EditorCamera = nullptr;
		Texture2D* m_SpriteSheet = nullptr;
		SubTexture2D* m_SubTextures2D[10];

		Camera* m_GameCamera = nullptr;
		Entity m_GameCameraEntity;

		Framebuffer* m_Framebuffer = nullptr;
		glm::vec2 m_ViewPortPanelSize = { 0.0f, 0.0f };

		int currentFPSDebugIndex = 0;
		float debugFPS[FPS_DEBUG_COUNT];
		float debugMS[FPS_DEBUG_COUNT];
		bool m_UpdateFPS = true;
		float m_AverageFPS = 0;
		float m_AverageMS = 0;

		bool m_VSync;
		bool m_ViewPortFocused = false;
		bool m_ViewPortHovered = false;

	private:

		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();

	};
}
