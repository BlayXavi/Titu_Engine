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

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetInversedProjectionMatrix() const { return m_InversedProjectionMatrix; }

		void SetProjectionType(Projection projection) { m_ProjectionType = projection; RecalculateProjectionMatrix(); }
		Projection GetProjectionType() const { return m_ProjectionType; }

		glm::vec3 ScreenSpacePosToWorldPos(const float& x, const float& y, const glm::mat4& model);
		glm::vec3 ClipSpacePosToWorldSpace(const float& x, const float& y, const glm::mat4& model);

		const float GetAspectRatio() const { return m_AspectRatio; }

		virtual void SetOrthographicProjection(float left, float right, float up, float down);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjectionMatrix(); }

		float GetFOV() { return m_FieldOfView; }
		void SetFOV(float fov) { m_FieldOfView = fov; RecalculateProjectionMatrix(); }

		float GetNearPlane() { return m_NearPlane; }
		void SetNearPlane(float plane) { m_NearPlane = plane; RecalculateProjectionMatrix(); }

		float GetFarPlane() { return m_FarPlane; }
		void SetFarPlane(float plane) { m_FarPlane = plane; RecalculateProjectionMatrix(); }

	protected:

		Projection m_ProjectionType = Projection::ORTHOGRAPHIC;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_InversedProjectionMatrix = glm::mat4(1.0f);

		float m_NearPlane;
		float m_FarPlane;

		float m_AspectRatio;

		float m_OrthographicSize;

		float m_FieldOfView;

		virtual void RecalculateProjectionMatrix();
	};

	class TransformedCamera : public Camera
	{
	public:
		TransformedCamera();
		virtual ~TransformedCamera() = default;

		void SetPosition(glm::vec3 position) { m_Position = position; RecalculateViewMatrix(); }
		const glm::vec3& GetPosition() const { return m_Position; }

		const glm::mat4& GetInversedViewMatrix() const { return m_InversedViewMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		float GetZRotation() const { return m_Rotation; }
		void SetZRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		virtual void SetOrthographicProjection(float left, float right, float up, float down) override;

	protected:
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_InversedViewMatrix = glm::mat4(1.0f);

		void RecalculateViewProjectionMatrix();
		void RecalculateViewMatrix();

		float m_Rotation;
		glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);

		virtual void RecalculateProjectionMatrix() override;

	};
}
