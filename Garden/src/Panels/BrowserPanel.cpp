#include "grpch.h"
#include "BrowserPanel.h"

#include <imgui/imgui.h>

namespace Gravel {

	// needs to change to project location
	extern const std::filesystem::path g_resPath = "res";

	BrowserPanel::BrowserPanel()
		: m_currentDirectory(g_resPath)
	{
		//consider texture atlas
		m_folderIcon = Texture2D::Create("res/ui/icons/icon-folder.png");
		m_fileIcon = Texture2D::Create("res/ui/icons/icon-file.png");
	}

	void BrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("File Browser");

		if (m_currentDirectory != std::filesystem::path(g_resPath))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 50.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_resPath);
			std::string filenameString = relativePath.filename().string();
			ImGui::PushID(filenameString.c_str());
			Shared<Texture2D> icon = directoryEntry.is_directory() ? m_folderIcon : m_fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_currentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();

		}

		ImGui::Columns(1);

		//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		//ImGui::SliderFloat("Padding", &padding, 0, 32);


		ImGui::End();
	}

}