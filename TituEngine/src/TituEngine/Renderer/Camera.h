#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TituEngine
{
	class Camera
	{
	public:

		enum class Projection { ORTHOGRAPHIC = 0, PERSPECTIVE = 1 };

		Camera();
		virtual ~Camera() = default;

		void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetInversedViewMatrix() const { return m_InversedViewMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		inline virtual float GetZRotation() const = 0;
		virtual void SetZRotation(float rotation) = 0;

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const float GetAspectRatio() const { return m_AspectRatio; }

	protected:

		Projection m_ProjectionTYpe = Projection::ORTHOGRAPHIC;

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_InversedViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

		float m_AspectRatio;

		void RecalculateViewProjectionMatrix();
		virtual void RecalculateViewMatrix() = 0;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		OrthographicCamera(float aspectRatio);

		void SetProjection(float left, float right, float bottom, float top);

		inline float GetZRotation() const override { return m_Rotation; }
		void SetZRotation(float rotation) override { m_Rotation = rotation; RecalculateViewMatrix(); }

	protected:
		float m_Rotation = 0.0f;

		virtual void RecalculateViewMatrix() override;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, glm::ivec2 aspectRatio, float nearPlane, float farPlane);

	protected:
		float m_FOV = 65.0f;
		glm::ivec2 m_AspectRatio = { 16,9 };
		float m_NearPlane = 10;
		float m_FarPlane = 600;
	};
}