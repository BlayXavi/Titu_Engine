#include "tepch.h"
#include "ContentBrowserPanel.h"

#include "imgui/imgui.h"

#include "TituEngine/Core/FilesystemUtilities.h"

namespace TituEngine
{
	ContentBrowserPanel::ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::DrawDirectoryTree(std::string directory)
	{
		TE_PROFILE_PROFILE_FUNC();

		for (std::filesystem::directory_entry directoryEntry : std::filesystem::directory_iterator(directory))
		{
			const std::filesystem::path& directoryPath = directoryEntry.path();
			const std::filesystem::path& relativePath = std::filesystem::relative(directoryPath, s_DefaultAssetPath);

			std::string directoryPathString = directoryPath.string();
			std::string relativePathString = relativePath.filename().string();

			ImGui::PushID(relativePathString.c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			bool isDirectory = directoryEntry.is_directory();

			if (isDirectory)
			{
				bool open = ImGui::TreeNodeEx(relativePathString.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
				ImGui::TableNextColumn();
				ImGui::TextDisabled("--");
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("Folder");
				if (open)
				{
					DrawDirectoryTree(directoryEntry.path().string());
					ImGui::TreePop();
				}
			}
			else
			{
				std::string extension = relativePath.extension().string();
				const char* extension_cstr = extension.c_str();

				ImGui::TreeNodeEx(relativePathString.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

				if (extension.compare(s_TituSceneExtension) == 0)
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("CONTENT_BROWSER_SCENE_ITEM", directoryPathString.c_str(), (directoryPathString.size() + 1) * sizeof(const char));
						ImGui::EndDragDropSource();
					}
				}
				else if (extension.compare(s_PNGExtension) == 0 || extension.compare(s_JPGExtension) == 0 || extension.compare(s_TGAExtension) == 0)
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("CONTENT_BROWSER_SCENE_SPRITE", directoryPathString.c_str(), (directoryPathString.size() + 1) * sizeof(const char));
						ImGui::EndDragDropSource();
					}
				}
				else if (extension.compare(s_ObjExtension) == 0 || extension.compare(s_FBXExtension) == 0)
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("CONTENT_BROWSER_SCENE_MODEL", directoryPathString.c_str(), (directoryPathString.size() + 1) * sizeof(const char));
						ImGui::EndDragDropSource();
					}
				}
				else if (extension.compare(s_ShaderExtension) == 0)
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("CONTENT_BROWSER_SCENE_SHADER", directoryPathString.c_str(), (directoryPathString.size() + 1) * sizeof(const char));
						ImGui::EndDragDropSource();
					}
				}

				ImGui::TableNextColumn();
				ImGui::Text("%d", std::filesystem::file_size(directoryPath));
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(extension_cstr);

			}

			ImGui::PopID();
		}
	}

	void ContentBrowserPanel::OnImGuiRender(bool& showWindow)
	{
		TE_PROFILE_PROFILE_FUNC();

		ImGui::Begin("Content Browser", &showWindow);

		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg;
		if (ImGui::BeginTable("3ways", 3, flags))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 6.0f * 12.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 6.0f * 18.0f);
			ImGui::TableHeadersRow();

			DrawDirectoryTree(s_DefaultAssetPath.string());

			ImGui::EndTable();
		}

		ImGui::End();
	}
}
