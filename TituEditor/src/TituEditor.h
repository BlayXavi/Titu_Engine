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
#include "Editor/ContentBrowserPanel.h"

#include "Editor/Utils/TituEditorUtilities.h"

#include <filesystem>

#define FPS_DEBUG_COUNT 120

namespace TituEngine
{
	class TituEditorLayer : public Layer
	{

	public:
		enum class EditorPlayState
		{
			Edit = 0,
			Play = 1
		};

		TituEditorLayer();
		~TituEditorLayer();

		void OnImGuiRender() override;

		void OnUpdate(Timestep ts) override;
		void OnEvent(Event& e) override;

		static TituEditorLayer::EditorPlayState GetEditorPlayState();

	public:
		static Signal<> OnSceneLoaded;

	private:
		static EditorPlayState s_EditorPlayState;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		Texture2D* m_PlayButtonState;
		Texture2D* m_PlayButtonIcon;
		Texture2D* m_StopButtonIcon;

		Scene* m_Scene = nullptr;
		SpriteRendererComponent* tempSpriteRendererComponent;

		Timestep currentTimeStep;

		EditorOrthographicCameraController* m_CameraController = nullptr;
		TransformedCamera* m_EditorCamera = nullptr;
		//Texture2D* m_SpriteSheet = nullptr;
		//SubTexture2D* m_SubTextures2D[10];

		Camera* m_GameCamera = nullptr;
		Entity m_GameCameraEntity;

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

		glm::ivec2 m_ContentRegionAvail;
		glm::ivec2 m_ViewportSize;
		glm::ivec2 m_AbsoluteViewportStartPos;
		glm::ivec2 m_MouseViewportPos;
		glm::ivec2 m_MouseViewportPosYInverted;
		int32_t m_LastPixelIDHovered;

		bool m_UsingGuizmo = false;
		bool m_HoveringGuizmo = false;

		EditorSnapValues m_SnapValues;

		Entity light;

	private:
		void SetEditorPlayState(EditorPlayState newPlayState);

		TRANSFORM_MANIPULATION_OPERATION m_SelectedTransformManipulation;
		COORDINATE_SYSTEM m_SelectedCoordinateSystem;
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);

		void Toolbar();

		void NewScene();
		void OpenScene();
		void OpenScene(std::filesystem::path path);
		void SaveScene();

	};
}
