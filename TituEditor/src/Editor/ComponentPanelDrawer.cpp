#include "tepch.h"
#include "ComponentPanelDrawer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <glm/gtx/matrix_decompose.hpp>


namespace TituEngine
{
	bool ComponentPanelDrawer::DrawVec3(std::string label, glm::vec3& values)
	{
		bool modified = false;

		float resetValue = 0.0f;
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		resetValue = 1.0f;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			modified = true;

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return modified;
	}

	void ComponentPanelDrawer::DrawTransformComponent(TransformComponent& transform)
	{
		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth, "Camera Component"))
		{
			glm::vec3 translation = transform.GetTranslation();
			bool modified = DrawVec3("Position", translation);
			glm::vec3 rotation = transform.GetRotation();
			modified |= DrawVec3("Rotation", rotation);
			glm::vec3 scale = transform.GetScale();
			modified |= DrawVec3("Scale", scale);
			if (modified)
				transform.SetTranslationAndRotationAndScale(translation, rotation, scale);
			ImGui::TreePop();
		}
	}

	void ComponentPanelDrawer::DrawCamera(Camera& c)
	{
		const char* projectionTypes[] = { "Orothographic", "Perspective" };
		static int currentItemSelected = (int)c.GetProjectionType();

		if (ImGui::Combo("Projection Type", &currentItemSelected, projectionTypes, 2))
			c.SetProjectionType((Camera::Projection)currentItemSelected);

		if (c.GetProjectionType() == Camera::Projection::PERSPECTIVE)
		{
			static float fov = c.GetFOV();
			if (ImGui::DragFloat("Field of View", &fov, 1.0f, 0.1f, 180.0f))
				c.SetFOV(fov);
		}
		else
		{
			static float oSize = c.GetOrthographicSize();
			if (ImGui::DragFloat("Orthographic Size", &oSize, 1.0f, 1.0f, 100.0f))
				c.SetOrthographicSize(oSize);
		}

		static float nearPlane = c.GetNearPlane();
		if (ImGui::DragFloat("NearPlane", &nearPlane, 1.0f, -100.0f, 1000.0f))
			c.SetNearPlane(nearPlane);

		static float farPlane = c.GetFarPlane();
		if (ImGui::DragFloat("FarPlane", &farPlane, 1.0f, -100.0f, 1000.0f))
			c.SetFarPlane(farPlane);
	}


	void ComponentPanelDrawer::DrawCameraComponent(CameraComponent& cameraComponent)
	{
		if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth, "Camera Component"))
		{
			Camera& c = cameraComponent.Camera;
			DrawCamera(c);
			ImGui::TreePop();
		}
	}
}