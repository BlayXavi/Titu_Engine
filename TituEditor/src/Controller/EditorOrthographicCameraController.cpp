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

	int mouseButton = -1;
	if (Input::IsAnyButtonPressed(mouseButton))
	{
		glm::vec3 eye = m_EditorCamera->GetPosition();
		glm::vec3 center = m_EditorCamera->GetCenter();
		glm::vec3 dir = m_EditorCamera->GetDirection();
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(up, dir));
		up = glm::normalize(glm::cross(dir, right));

		float speedDelta = m_CameraSpeed * ts;

		if (Input::IsKeyPressed(TE_KEY_LEFT_SHIFT))
			speedDelta *= 3.0f;

		if (Input::IsKeyPressed(TE_KEY_W))
		{
			eye += dir * speedDelta;
			center += dir * speedDelta;
		}
		if (Input::IsKeyPressed(TE_KEY_S))
		{
			eye -= dir * speedDelta;
			center -= dir * speedDelta;
		}
		if (Input::IsKeyPressed(TE_KEY_A))
		{
			eye += right * speedDelta;
			center += right * speedDelta;
		}
		if (Input::IsKeyPressed(TE_KEY_D))
		{
			eye -= right * speedDelta;
			center -= right * speedDelta;
		}

		std::pair<float, float> mouseDelta = Input::GetMouseDeltaPosition();
		if (mouseButton == 0)
		{
			center += right * mouseDelta.first * -speedDelta;
			center += up * mouseDelta.second * speedDelta;
		}
		else if (mouseButton == 1)
		{
			//orbit (like span), invert movement
			
		}
		else if (mouseButton == 2)
		{
			//span, invert movement
			eye -= right * mouseDelta.first * -speedDelta;
			center -= right * mouseDelta.first * -speedDelta;

			eye -= up * mouseDelta.second * speedDelta;
			center -= up * mouseDelta.second * speedDelta;
		}

		m_EditorCamera->LookAt(eye, center);
	}
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
	if (m_EditorCamera->GetProjectionType() == Camera::Projection::ORTHOGRAPHIC)
	{
		float m_ZoomLevel = m_EditorCamera->GetOrthographicSize();
		m_ZoomLevel -= e.GetYOffset();
		m_EditorCamera->SetOrthographicSize(m_ZoomLevel);
	}
	else
	{
		glm::vec3 eye = m_EditorCamera->GetPosition();
		glm::vec3 center = m_EditorCamera->GetCenter();
		glm::vec3 dir = m_EditorCamera->GetDirection();

		eye += dir * e.GetYOffset();
		center += dir * e.GetYOffset();

		m_EditorCamera->LookAt(eye, center);
	}
	return false;
}

