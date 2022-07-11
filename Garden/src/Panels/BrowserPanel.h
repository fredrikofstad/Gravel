#pragma once

#include <filesystem>

namespace Gravel {

	class BrowserPanel
	{
	public:
		BrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_currentDirectory;
	};

}