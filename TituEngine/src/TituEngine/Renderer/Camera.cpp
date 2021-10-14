#include "tepch.h"
#include "Camera.h"

namespace TituEngine
{
	//-------------------------------------------CAMERA-------------------------------------
	Camera::Camera()
		: m_ProjectionMatrix(1.0f), m_AspectRatio(1.0f), m_NearPlane(-10.0f), m_FarPlane(10.0f), m_OrthographicSize(1.0f)
	{
	}

	glm::vec3 Camera::ScreenSpacePosToWorldPos(const float& x, const float& y, const glm::mat4& model)
	{
		std::pair<float, float> windowsSize = { Application::Instance().GetWindow().GetWidth() * 0.5f,  Application::Instance().GetWindow().GetHeight() * 0.5f };

		return ClipSpacePosToWorldSpace(((x - windowsSize.first) / windowsSize.first), (y - windowsSize.second) / (windowsSize.second * -1.0f), model);
	}

	inline glm::vec3 Camera::ClipSpacePosToWorldSpace(const float& x, const float& y, const glm::mat4& model)
	{
		glm::vec4 tempPos = { x, y, 0.0f, 1.0f };
		tempPos = GetInversedProjectionMatrix() * tempPos;
		tempPos = model * tempPos;
		return tempPos;
	}

	void Camera::SetOrthographicProjection(float left, float right, float up, float down)
	{
		m_ProjectionType = Projection::ORTHOGRAPHIC;
		m_ProjectionMatrix = glm::ortho(left, right, up, down, m_NearPlane, m_FarPlane);
	}

	//-------------------------------------------ORHTO CAMERA-------------------------------------
	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / float(height);
		RecalculateProjectionMatrix();
	}


	void Camera::RecalculateProjectionMatrix()
	{
		if (m_ProjectionType == Projection::ORTHOGRAPHIC)
		{
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_NearPlane, m_FarPlane);
		}
		else if (m_ProjectionType == Projection::PERSPECTIVE)
		{
			TE_ASSERT(false, "PERSPECTIVE PROJECTION NOT SUPPORTED YET.")
		}
	}

	//----------TransformedCamera

	TransformedCamera::TransformedCamera()
		: m_ViewMatrix(1.0f), m_InversedViewMatrix(1.0f), m_ViewProjectionMatrix(1.0f), m_Rotation(0.0f)
	{
	}

	void TransformedCamera::SetOrthographicProjection(float left, float right, float up, float down)
	{
		Camera::SetOrthographicProjection(left, right, up, down);
		RecalculateViewProjectionMatrix();
	}

	void TransformedCamera::RecalculateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_InversedViewMatrix;
	}

	void TransformedCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = transform;
		m_InversedViewMatrix = glm::inverse(transform);
		RecalculateViewProjectionMatrix();
	}

	void TransformedCamera::RecalculateProjectionMatrix()
	{
		Camera::RecalculateProjectionMatrix();
		RecalculateViewMatrix();
	}
}