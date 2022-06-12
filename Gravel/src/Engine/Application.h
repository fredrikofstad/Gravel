#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/LayerStack.h"
#include "Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

//TODO: fix folder structure
#include "Imgui/ImguiLayer.h"

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

		inline Window& GetWindow() { return *m_window; }

		inline static Application& Get() { return *s_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		std::unique_ptr<Window> m_window;
		ImguiLayer* m_imguiLayer;
		bool m_running = true;
		LayerStack m_layerStack;

		unsigned int m_vertexArray, m_vertexBuffer, m_indexBuffer;

		static Application* s_instance;
	};

	//defined in client
	Application* CreateApplication();

}
