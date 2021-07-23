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

class OrthographicCameraController
{
public:
	OrthographicCameraController();
	OrthographicCameraController(OrthographicCamera* camera);
	OrthographicCameraController(OrthographicCamera* camera, const glm::vec3& position);

	~OrthographicCameraController();

	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);

	void SetCamera(OrthographicCamera* camera);
	void SetPosition(const glm::vec3& position);

	void SetZoomMinMax(float min, float max);

private:
	bool OnMouseScrolled(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);

private:
	OrthographicCamera* m_Camera = nullptr;
	float m_ZoomLevel = 1.0f;
	float m_CameraSpeed = 1.0f;

	float m_ZoomLevelMin = 0.1f;
	float m_ZoomLevelMax = 10.0f;

	glm::vec3 m_CameraPosition;
};