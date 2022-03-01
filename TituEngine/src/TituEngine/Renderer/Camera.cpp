#include "tepch.h"
#include "Camera.h"
#include "TituEngine/Core/Application.h"
#include <glm/gtx/quaternion.hpp>
namespace TituEngine
{
	Camera::ActiveCameraData Camera::s_ActiveCamera;

	//-------------------------------------------CAMERA-------------------------------------
	Camera::Camera()
		: m_ProjectionMatrix(1.0f), m_AspectRatio(1.0f), m_NearPlane(0.001f), m_FarPlane(100.0f), m_OrthographicSize(1.0f), m_FieldOfView(90.0f)
	{
	}

	glm::vec3 Camera::ScreenSpacePosToWorldPos(const float& x, const float& y, const glm::mat4& model)const
	{
		std::pair<float, float> windowsSize = { Application::Instance().GetWindow().GetWidth() * 0.5f,  Application::Instance().GetWindow().GetHeight() * 0.5f };

		return ClipSpacePosToWorldSpace(((x - windowsSize.first) / windowsSize.first), (y - windowsSize.second) / (windowsSize.second * -1.0f), model);
	}

	inline glm::vec3 Camera::ClipSpacePosToWorldSpace(const float& x, const float& y, const glm::mat4& model) const
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
		SetAspectRatio((float)width / float(height));
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

			m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearPlane, m_FarPlane);
		}
	}

	//----------TransformedCamera

	TransformedCamera::TransformedCamera()
		: m_ViewMatrix(1.0f), m_ViewProjectionMatrix(1.0f), m_Eye(0.0f), m_Center(0.0f)
	{
	}

	void TransformedCamera::SetOrthographicProjection(float left, float right, float up, float down)
	{
		Camera::SetOrthographicProjection(left, right, up, down);
		RecalculateViewProjectionMatrix();
	}

	void TransformedCamera::RecalculateViewProjectionMatrix()
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void TransformedCamera::UpdateViewMatrix()
	{
		m_Direction = -glm::normalize(m_Center - m_Eye);
		glm::vec3 right = glm::cross(m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
		right = glm::normalize(right);
		glm::vec3 up = glm::cross(right, m_Direction);
		up = glm::normalize(up);
		glm::mat4 r(1.0f);
		r[0][0] = right.x;
		r[1][0] = right.y;
		r[2][0] = right.z;
		r[0][1] = up.x;
		r[1][1] = up.y;
		r[2][1] = up.z;
		r[0][2] = m_Direction.x;
		r[1][2] = m_Direction.y;
		r[2][2] = m_Direction.z;
		glm::mat4 t = glm::translate(glm::mat4(1.0f), -m_Eye);

		m_ViewMatrix = r * t;
		RecalculateViewProjectionMatrix();
	}
}