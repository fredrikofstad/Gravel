#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/LayerStack.h"
#include "Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Gravel {

	class GRAVEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
	};

	//defined in client
	Application* CreateApplication();

}
