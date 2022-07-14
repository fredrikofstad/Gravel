#include "grpch.h"
#include "BrowserPanel.h"

#include <imgui/imgui.h>

namespace Gravel {

	// Once we have projects, change this
	static const std::filesystem::path s_resPath = "res";

	BrowserPanel::BrowserPanel()
		: m_currentDirectory(s_resPath)
	{
		//consider texture atlas
		m_folderIcon = Texture2D::Create("res/icons/icon-folder.png");
		m_fileIcon = Texture2D::Create("res/icons/icon-file.png");
	}

	void BrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("File Browser");

		if (m_currentDirectory != std::filesystem::path(s_resPath))
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
			auto relativePath = std::filesystem::relative(path, s_resPath);
			std::string filenameString = relativePath.filename().string();
			Shared<Texture2D> icon = directoryEntry.is_directory() ? m_folderIcon : m_fileIcon;
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_currentDirectory /= path.filename();
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);


		ImGui::End();
	}

}