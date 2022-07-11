#include "grpch.h"
#include "BrowserPanel.h"

#include <imgui/imgui.h>

namespace Gravel {

	// Once we have projects, change this
	static const std::filesystem::path s_resPath = "res";

	BrowserPanel::BrowserPanel()
		: m_currentDirectory(s_resPath)
	{
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

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_resPath);
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_currentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{
				}
			}
		}

		ImGui::End();
	}

}