#include "tepch.h"
#include "OrthographicCameraController.h"
#include <functional>

using namespace TituEngine;

OrthographicCameraController::OrthographicCameraController()
	: m_Camera(nullptr), m_CameraPosition(glm::vec3(0.0f)), m_ZoomLevelMin(0.1f), m_ZoomLevelMax(10.0f)
{
}

OrthographicCameraController::OrthographicCameraController(OrthographicCamera* camera)
	: m_Camera(camera), m_CameraPosition(glm::vec3(0.0f)), m_ZoomLevelMin(0.1f), m_ZoomLevelMax(10.0f)
{

}


OrthographicCameraController::OrthographicCameraController(OrthographicCamera* camera, const glm::vec3& position)
	: m_Camera(camera), m_CameraPosition(position)
{

}

OrthographicCameraController::~OrthographicCameraController()
{
	m_Camera = nullptr;
}

void OrthographicCameraController::SetPosition(const glm::vec3& position)
{
	m_CameraPosition = position;
	m_Camera->SetPosition(position);
}

void OrthographicCameraController::SetZoomMinMax(float min, float max)
{
	m_ZoomLevelMin = min;
	m_ZoomLevelMin = max;
}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
	TE_PROFILE_PROFILE_FUNC();

	if (m_Camera == nullptr)
		return;

	glm::vec3 pos = m_CameraPosition;
	if (InputBridge::IsKeyPressed(TE_KEY_UP))
		pos.y += m_CameraSpeed * ts;
	else if (InputBridge::IsKeyPressed(TE_KEY_DOWN))
		pos.y -= m_CameraSpeed * ts;
	
	if (InputBridge::IsKeyPressed(TE_KEY_RIGHT))
		pos.x += m_CameraSpeed * ts;
	else if (InputBridge::IsKeyPressed(TE_KEY_LEFT))
		pos.x -= m_CameraSpeed * ts;

	SetPosition(pos);
}

void OrthographicCameraController::SetCamera(OrthographicCamera* camera)
{
	m_Camera = camera;
}

void OrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher eDispatcher(e);
	
	eDispatcher.Dispatch<MouseScrolledEvent>(std::bind(&OrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
}

void OrthographicCameraController::OnResize(uint32_t width, uint32_t height)
{
	float newAspectRatio = (float)width / (float)height;
	m_Camera->SetProjection(-newAspectRatio * m_ZoomLevel, newAspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_ZoomLevel += e.GetYOffset();
	m_ZoomLevel = std::clamp(m_ZoomLevel, m_ZoomLevelMin, m_ZoomLevelMax);
	float aspectRatio = m_Camera->GetAspectRatio();
	m_Camera->SetProjection(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	OnResize(e.GetWidth(), e.GetHeight());
	return false;
}
