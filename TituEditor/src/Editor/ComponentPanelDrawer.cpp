#include "tepch.h"
#include "ComponentPanelDrawer.h"

namespace TituEngine
{
	ImGuiTreeNodeFlags ComponentPanelDrawer::m_TreeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_Bullet;

	bool ComponentPanelDrawer::DrawVec3(std::string label, glm::vec3& values, const glm::vec3& resetValue, const float stepValue)
	{
		bool modified = false;

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
		//ImGui::Font 
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue.x;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, stepValue, 0.0f, 0.0f, "%.4f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue.y;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, stepValue, 0.0f, 0.0f, "%.4f"))
			modified = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue.z;
			modified = true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, stepValue, 0.0f, 0.0f, "%.4f"))
			modified = true;

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return modified;
	}

	void ComponentPanelDrawer::DrawCamera(Camera& c)
	{
		const char* projectionTypes[] = { "Orothographic", "Perspective" };
		static int currentItemSelected = 0;
		currentItemSelected = (int)c.GetProjectionType();

		if (ImGui::Combo("Projection Type", &currentItemSelected, projectionTypes, 2))
			c.SetProjectionType((Camera::Projection)currentItemSelected);

		if (c.GetProjectionType() == Camera::Projection::PERSPECTIVE)
		{
			float fov = c.GetFOV();
			if (ImGui::DragFloat("Field of View", &fov, 1.0f, 0.1f, 180.0f))
				c.SetFOV(fov);
		}
		else
		{
			float oSize = c.GetOrthographicSize();
			if (ImGui::DragFloat("Orthographic Size", &oSize, 1.0f, 1.0f, 100.0f))
				c.SetOrthographicSize(oSize);
		}

		float nearPlane = c.GetNearPlane();
		if (ImGui::DragFloat("NearPlane", &nearPlane, 1.0f, -100.0f, 1000.0f))
			c.SetNearPlane(nearPlane);

		float farPlane = c.GetFarPlane();
		if (ImGui::DragFloat("FarPlane", &farPlane, 1.0f, -100.0f, 1000.0f))
			c.SetFarPlane(farPlane);
	}

	/*template<typename C>
	void ComponentPanelDrawer::DrawComponent(Entity& e, C& Component)
	{
		if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), m_TreeNodeFlags, typeid(T).name()))
		{
			static bool removed = false;
			ComponentSettings(e, removed);
			if (!removed)
			{
				DrawComponentInternal(e, Component);
			}
		}
	}*/

	void ComponentPanelDrawer::DrawTagComponent(Entity& e, TagComponent& tag)
	{
		std::string& t = tag.Tag;
		static char buffer[32] = "";
		//memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), t.c_str());
		if (ImGui::InputText("##TAG", buffer, sizeof(buffer), ImGuiInputTextFlags_CharsNoBlank))
			t = std::string(buffer);
	}

	template<>
	void ComponentPanelDrawer::DrawComponentInternal<TransformComponent>(Entity& e, TransformComponent& transform)
	{
		glm::vec3 translation = transform.GetTranslation();
		bool modified = DrawVec3("Position", translation, { 0.0f, 0.0f, 0.0f }, 0.1f);
		glm::vec3 rotation = glm::degrees(transform.GetRotation());
		modified |= DrawVec3("Rotation", rotation, { 0.0f, 0.0f, 0.0f }, 0.1f);
		glm::vec3 scale = transform.GetScale();
		modified |= DrawVec3("Scale", scale, { 1.0f, 1.0f, 1.0f }, 01.f);
		if (modified)
			transform.SetTranslationAndRotationAndScale(translation, glm::radians(rotation), scale);
	}

	template<>
	void ComponentPanelDrawer::DrawComponentInternal<CameraComponent>(Entity& e, CameraComponent& cameraComponent)
	{
		Camera& c = cameraComponent.Camera;
		DrawCamera(c);
		if (ImGui::Button("Set as ActiveCamera"))
			cameraComponent.SetAsActiveCamera();
	}

	template<>
	void ComponentPanelDrawer::DrawComponentInternal<SpriteRendererComponent>(Entity& e, SpriteRendererComponent& spriteRenderer)
	{
		glm::vec4& color = spriteRenderer.Color;

		ImGui::ColorEdit4("Color", (float*)&color);

		ImGui::Button("Drop Texture Here", ImVec2(160.0f, 0.0f));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_SCENE_PNG"))
			{
				const char* path = (const char*)payload->Data;
				spriteRenderer.Texture = new SubTexture2D(Texture2D::Create(std::string(path)), { 0.0f, 0.0f }, { 1.0f, 1.0f });
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::DragFloat2("Tiling Factor", &spriteRenderer.TileSize[0], 0.1f, 0.0f, 10.0f);

	}

	template<>
	void ComponentPanelDrawer::DrawComponentInternal<ModelRendererComponent>(Entity& e, ModelRendererComponent& modelRenderer)
	{
		Model* model = modelRenderer.GetModel();
		//uint32_t textures = mesh->GetTexturesCount();

		ImGui::Button("Drop Model here", ImVec2(160.0f, 0.0f));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_SCENE_MODEL"))
			{
				const char* path = (const char*)payload->Data;
				Model* model = Model::Create(path);
				if (model != nullptr)
					modelRenderer.SetModel(model);
			}
		}

	}

	template<typename C>
	void ComponentPanelDrawer::DrawComponentInternal(Entity& e, C& Component)
	{
		static_assert(false);
	}

}