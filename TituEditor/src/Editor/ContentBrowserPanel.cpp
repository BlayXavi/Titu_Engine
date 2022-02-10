#include "tepch.h"
#include "ContentBrowserPanel.h"

#include "imgui/imgui.h"

namespace TituEngine
{
	static const std::filesystem::path s_DefaultAssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentPath(s_DefaultAssetPath)
	{
	}

	void ContentBrowserPanel::DrawDirectoryTree(std::string directory)
	{
		for (std::filesystem::directory_entry directoryEntry : std::filesystem::directory_iterator(directory))
		{
			const std::filesystem::path& directoryPath = directoryEntry.path();
			const std::filesystem::path& relativePath = std::filesystem::relative(directoryPath, s_DefaultAssetPath);

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
				ImGui::TreeNodeEx(relativePathString.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
				ImGui::TableNextColumn();
				ImGui::Text("%d", std::filesystem::file_size(directoryPath));
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(directoryPath.filename().extension().string().c_str());
			}

			ImGui::PopID();
		}
	}

	void ContentBrowserPanel::OnImGuiRender(bool& showWindow)
	{
		ImGui::Begin("Content Browser", &showWindow);

		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
		if (ImGui::BeginTable("3ways", 3, flags))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, m_ThumbnailSize * 12.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, m_ThumbnailSize * 18.0f);
			ImGui::TableHeadersRow();

			DrawDirectoryTree(m_CurrentPath.string());

			ImGui::EndTable();
		}

		ImGui::SliderFloat("Thumbnail Size", &m_ThumbnailSize, 1, 64);

		ImGui::End();
	}
}
