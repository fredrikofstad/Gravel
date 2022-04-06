#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace Gravel {

	class GRAVEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};

	//defined in client
	Application* CreateApplication();

}
