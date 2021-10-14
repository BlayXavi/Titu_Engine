#include "tepch.h"
#include "OrthographicCameraController.h"
#include <functional>

using namespace TituEngine;

EditorOrthographicCameraController::EditorOrthographicCameraController()
	: m_EditorCamera(nullptr), m_CameraPosition(glm::vec3(0.0f)), m_ZoomLevelMin(0.1f), m_ZoomLevelMax(10.0f)
{
}

EditorOrthographicCameraController::EditorOrthographicCameraController(OrthographicCamera* camera)
	: m_EditorCamera(camera), m_CameraPosition(glm::vec3(0.0f)), m_ZoomLevelMin(0.1f), m_ZoomLevelMax(10.0f)
{

}


EditorOrthographicCameraController::EditorOrthographicCameraController(OrthographicCamera* camera, const glm::vec3& position)
	: m_EditorCamera(camera), m_CameraPosition(position)
{

}

EditorOrthographicCameraController::~EditorOrthographicCameraController()
{
	m_EditorCamera = nullptr;
}

void EditorOrthographicCameraController::SetPosition(const glm::vec3& position)
{
	m_CameraPosition = position;
	m_EditorCamera->SetPosition(position);
}

void EditorOrthographicCameraController::SetZoomMinMax(float min, float max)
{
	m_ZoomLevelMin = min;
	m_ZoomLevelMin = max;
}

void EditorOrthographicCameraController::OnUpdate(Timestep ts)
{
	TE_PROFILE_PROFILE_FUNC();

	if (m_EditorCamera == nullptr)
		return;

	glm::vec3 pos = m_CameraPosition;
	if (Input::IsKeyPressed(TE_KEY_UP))
		pos.y += m_CameraSpeed * ts;
	else if (Input::IsKeyPressed(TE_KEY_DOWN))
		pos.y -= m_CameraSpeed * ts;
	
	if (Input::IsKeyPressed(TE_KEY_RIGHT))
		pos.x += m_CameraSpeed * ts;
	else if (Input::IsKeyPressed(TE_KEY_LEFT))
		pos.x -= m_CameraSpeed * ts;

	SetPosition(pos);
}

void EditorOrthographicCameraController::SetCamera(OrthographicCamera* camera)
{
	m_EditorCamera = camera;
}

void EditorOrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher eDispatcher(e);
	
	eDispatcher.Dispatch<MouseScrolledEvent>(std::bind(&EditorOrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
	eDispatcher.Dispatch<WindowResizeEvent>	(std::bind(&EditorOrthographicCameraController::OnWindowResized, this, std::placeholders::_1));
}

bool EditorOrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	m_ZoomLevel += e.GetYOffset();
	m_ZoomLevel = std::clamp(m_ZoomLevel, m_ZoomLevelMin, m_ZoomLevelMax);
	float aspectRatio = m_EditorCamera->GetAspectRatio();
	m_EditorCamera->SetProjection(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}

bool EditorOrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
	float newAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
	m_EditorCamera->SetProjection(-newAspectRatio * m_ZoomLevel, newAspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	return false;
}
