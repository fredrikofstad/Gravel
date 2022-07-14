#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"

namespace Gravel {

	class BrowserPanel
	{
	public:
		BrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;

		Shared<Texture2D> m_folderIcon;
		Shared<Texture2D> m_fileIcon;
	};

}