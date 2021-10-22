#include "tepch.h"
#include "ComponentPanelDrawer.h"
#include "imgui/imgui.h"

namespace TituEngine
{
	void ComponentPanelDrawer::DrawTransform(TransformComponent& transform)
	{

	}

	void ComponentPanelDrawer::DrawCameraComponent(CameraComponent& cameraComponent)
	{
		if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth, "Camera Component"))
		{
			const char* projectionTypes[] = { "Orothographic", "Perspective" };
			static int currentItemSelected = (int)cameraComponent.Camera.GetProjectionType();

			Camera& c = cameraComponent.Camera;

			if (ImGui::Combo("Projection Type", &currentItemSelected, projectionTypes, 2))
				c.SetProjectionType((Camera::Projection)currentItemSelected);

			static float fov = c.GetFOV();
			if (ImGui::DragFloat("Field of View", &fov, 1.0f, 0.1f, 180.0f))
				c.SetFOV(fov);

			static float nearPlane = c.GetNearPlane();
			if (ImGui::DragFloat("NearPlane", &nearPlane, 1.0f, 0.001f, 1000.0f))
				c.SetNearPlane(nearPlane);

			static float farPlane = c.GetFarPlane();
			if (ImGui::DragFloat("FarPlane", &farPlane, 1.0f, 0.001f, 1000.0f))
				c.SetFarPlane(farPlane);

			ImGui::TreePop();
		}
	}
}