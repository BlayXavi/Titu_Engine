#include "tepch.h"
#include "Camera.h"

namespace TituEngine
{
	//-------------------------------------------CAMERA-------------------------------------
	Camera::Camera()
		: m_ViewMatrix(1.0f), m_ProjectionMatrix(1.0f), m_ViewProjectionMatrix(1.0f)
	{
	}

	void Camera::RecalculateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	//-------------------------------------------ORHTO CAMERA-------------------------------------
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		RecalculateViewProjectionMatrix();
	}

	//-------------------------------------------PERSPECTIVE CAMERA-------------------------------------
	PerspectiveCamera::PerspectiveCamera(float fov, glm::ivec2 aspectRatio, float nearPlane, float farPlane)
	{

	}
}