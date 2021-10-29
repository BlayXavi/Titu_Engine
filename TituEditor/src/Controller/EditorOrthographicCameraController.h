#pragma once

#include "TituEngine/Input/Input.h"
#include "TituEngine/Renderer/Camera.h"
#include "TituEngine/Core/Timestep.h"

#include "TituEngine/Events/Event.h"
#include "TituEngine/Events/KeyEvent.h"
#include "TituEngine/Events/MouseEvent.h"
#include "TituEngine/Events/ApplicationEvent.h"
#include "TituEngine/Input/KeyboardMouseCodes.h"

using namespace TituEngine;

class EditorOrthographicCameraController
{
public:
	EditorOrthographicCameraController();
	EditorOrthographicCameraController(TransformedCamera* camera);
	EditorOrthographicCameraController(TransformedCamera* camera, const glm::vec3& position);

	~EditorOrthographicCameraController();

	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);

	//void OnResize(uint32_t width, uint32_t height);

	void SetCamera(TransformedCamera* camera);
	void SetPosition(const glm::vec3& position);

	void SetZoomLevel(float zoom) { m_EditorCamera->SetOrthographicSize(zoom); }
	inline const float& GetZoomLevel() { return m_EditorCamera->GetOrthographicSize(); }

private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);

private:
	TransformedCamera* m_EditorCamera = nullptr;
	float m_CameraSpeed = 1.0f;

	glm::vec3 m_CameraPosition;
};