#pragma once

#include "TituEngine/Math/Math.h"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TituEngine
{
	class Camera
	{
	public:

		struct ActiveCameraData
		{
		public:
			ActiveCameraData() : camera(nullptr), viewMatrix(nullptr), invertViewMatrix(false) { }
			ActiveCameraData(const ActiveCameraData& data) = default;
			ActiveCameraData(Camera* cam, glm::mat4* viewMat, bool invertViewMatrix) : camera(cam), viewMatrix(viewMat), invertViewMatrix(invertViewMatrix) { }

			Camera* GetCamera() const { return camera; }

			//returns the inversed of the related mat4
			glm::mat4 GetViewMatrix() { return invertViewMatrix ? glm::inverse(*viewMatrix) : *viewMatrix; }
			glm::mat4 GetProjectionMatrix() { return camera->GetProjectionMatrix(); }
			glm::mat4 GetViewProjectionMatrix() { return camera->GetProjectionMatrix() * GetViewMatrix(); }
			glm::vec3 GetPosition() const
			{
				glm::mat4 vm = (*viewMatrix);
				return glm::vec3(vm[3][0], vm[3][1], vm[3][2]);
			}

		private:
			Camera* camera;
			glm::mat4* viewMatrix;
			bool invertViewMatrix;
		};

		enum class Projection { ORTHOGRAPHIC = 0, PERSPECTIVE = 1 };

		Camera();
		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetInversedProjectionMatrix() const { return m_InversedProjectionMatrix; }

		void SetProjectionType(Projection projection) { m_ProjectionType = projection; RecalculateProjectionMatrix(); }
		Projection GetProjectionType() const { return m_ProjectionType; }

		glm::vec3 ScreenSpacePosToWorldPos(const float& x, const float& y, const glm::mat4& model) const;
		glm::vec3 ClipSpacePosToWorldSpace(const float& x, const float& y, const glm::mat4& model) const;

		const float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

		virtual void SetOrthographicProjection(float left, float right, float up, float down);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size)
		{
			m_OrthographicSize = size;
			if (m_OrthographicSize <= 1.0f)
				m_OrthographicSize = 1.0f;
			RecalculateProjectionMatrix();
		}

		float GetFOV() { return m_FieldOfView; }
		void SetFOV(float fov) { m_FieldOfView = fov; RecalculateProjectionMatrix(); }

		float GetNearPlane() { return m_NearPlane; }
		void SetNearPlane(float plane) { m_NearPlane = plane; RecalculateProjectionMatrix(); }

		float GetFarPlane() { return m_FarPlane; }
		void SetFarPlane(float plane) { m_FarPlane = plane; RecalculateProjectionMatrix(); }

		static void SetActiveCamera(Camera* const camera, glm::mat4* const viewMatrix, const bool invertViewMatrix)
		{
			Camera::s_ActiveCamera = Camera::ActiveCameraData(camera, viewMatrix, invertViewMatrix);
		}
		static ActiveCameraData GetActiveCamera() { return s_ActiveCamera; }

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

		static Camera::ActiveCameraData s_ActiveCamera;
	};

	class TransformedCamera : public Camera
	{
	public:
		TransformedCamera();
		virtual ~TransformedCamera() = default;

		void SetPosition(const glm::vec3& position)
		{
			m_Eye = position;
			LookAt(m_Center);
		}

		void LookAt(const glm::vec3& position, const glm::vec3& target)
		{
			m_Eye = position;
			m_Center = target;
			UpdateViewMatrix();
		}

		void LookAt(const glm::vec3& lookPos)
		{
			m_Center = lookPos;
			UpdateViewMatrix();
		}

		glm::vec3 GetPosition() const { return m_Eye; }
		glm::vec3 GetCenter() const { return m_Center; }
		glm::vec3 GetDirection() const { return m_Direction; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


		virtual void SetOrthographicProjection(float left, float right, float up, float down) override;

		void SetAsActiveCamera()
		{
			Camera::SetActiveCamera(this, &m_ViewMatrix, false);
		}

	protected:
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

		glm::vec3 m_Eye = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Center = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_Direction = glm::vec3(0.0f, 0.0f, 0.0f);

		void RecalculateViewProjectionMatrix();
		void UpdateViewMatrix();
	};
}
