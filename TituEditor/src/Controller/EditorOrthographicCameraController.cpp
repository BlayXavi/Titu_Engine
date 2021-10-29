#include "tepch.h"
#include "EditorOrthographicCameraController.h"
#include <functional>


EditorOrthographicCameraController::EditorOrthographicCameraController()
	: m_EditorCamera(nullptr), m_CameraPosition(glm::vec3(0.0f))
{
}

EditorOrthographicCameraController::EditorOrthographicCameraController(TransformedCamera* m_EditorCamera)
	: m_EditorCamera(m_EditorCamera), m_CameraPosition(glm::vec3(0.0f))
{

}


EditorOrthographicCameraController::EditorOrthographicCameraController(TransformedCamera* camera, const glm::vec3& position)
	: m_EditorCamera(m_EditorCamera), m_CameraPosition(position)
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

void EditorOrthographicCameraController::OnUpdate(Timestep ts)
{
	TE_PROFILE_PROFILE_FUNC();

	if (m_EditorCamera == nullptr)
		return;

	glm::vec3 pos = m_CameraPosition;
	if (Input::IsKeyPressed(TE_KEY_UP) || Input::IsKeyPressed(TE_KEY_W))
		pos.y += m_CameraSpeed * ts;
	else if (Input::IsKeyPressed(TE_KEY_DOWN) || Input::IsKeyPressed(TE_KEY_S))
		pos.y -= m_CameraSpeed * ts;
	
	if (Input::IsKeyPressed(TE_KEY_RIGHT) || Input::IsKeyPressed(TE_KEY_D))
		pos.x += m_CameraSpeed * ts;
	else if (Input::IsKeyPressed(TE_KEY_LEFT) || Input::IsKeyPressed(TE_KEY_A))
		pos.x -= m_CameraSpeed * ts;

	if (Input::IsButtonMousePressed(TE_MOUSE_BUTTON_3))
	{
		pos.x += Input::MouseDeltaX() * ts;
		pos.y += Input::MouseDeltaY() * -ts;
	}

	SetPosition(pos);
}

void EditorOrthographicCameraController::SetCamera(TransformedCamera* camera)
{
	m_EditorCamera = camera;
}

void EditorOrthographicCameraController::OnEvent(Event& e)
{
	EventDispatcher eDispatcher(e);
	
	eDispatcher.Dispatch<MouseScrolledEvent>(std::bind(&EditorOrthographicCameraController::OnMouseScrolled, this, std::placeholders::_1));
}

//void EditorOrthographicCameraController::OnResize(uint32_t width, uint32_t height)
//{
//	float newAspectRatio = (float)width / (float)height;
//	m_EditorCamera->SetViewportSize(width, height);
//}

bool EditorOrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	float m_ZoomLevel = m_EditorCamera->GetOrthographicSize();
	m_ZoomLevel += e.GetYOffset();
	m_EditorCamera->SetOrthographicSize(m_ZoomLevel);
	return false;
}
//
//bool EditorOrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
//{
//	OnResize(e.GetWidth(), e.GetHeight());
//	return false;
//}
