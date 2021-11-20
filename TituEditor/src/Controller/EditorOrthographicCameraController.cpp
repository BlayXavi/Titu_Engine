#include "tepch.h"
#include "EditorOrthographicCameraController.h"
#include <functional>


EditorOrthographicCameraController::EditorOrthographicCameraController()
	: m_EditorCamera(nullptr)
{
}

EditorOrthographicCameraController::EditorOrthographicCameraController(TransformedCamera* m_EditorCamera)
	: m_EditorCamera(m_EditorCamera)
{

}


EditorOrthographicCameraController::EditorOrthographicCameraController(TransformedCamera* camera, const glm::vec3& position)
	: m_EditorCamera(m_EditorCamera)
{

}

EditorOrthographicCameraController::~EditorOrthographicCameraController()
{
	m_EditorCamera = nullptr;
}

void EditorOrthographicCameraController::SetPosition(const glm::vec3& position)
{
	m_EditorCamera->SetPosition(position);
}

void EditorOrthographicCameraController::OnUpdate(Timestep ts)
{
	TE_PROFILE_PROFILE_FUNC();

	if (m_EditorCamera == nullptr)
		return;

	glm::vec3 pos = m_EditorCamera->GetPosition();
	if (Input::IsAnyButtonPressed())
	{
		if (Input::IsKeyPressed(TE_KEY_W))
		{
			pos += (m_EditorCamera->GetDirection() * (float)ts);
		}
		if (Input::IsKeyPressed(TE_KEY_S))
		{
			pos -= m_EditorCamera->GetDirection() * (float)ts;
		}
	}

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

	eDispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent mse)->bool {return this->OnMouseScrolled(mse); });
}

bool EditorOrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
	float m_ZoomLevel = m_EditorCamera->GetOrthographicSize();
	m_ZoomLevel -= e.GetYOffset();
	m_EditorCamera->SetOrthographicSize(m_ZoomLevel);
	return false;
}

