#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Imgui/ImguiLayer.h"



namespace Gravel {

	class Application
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
		bool OnWindowResize(WindowResizeEvent& event);


		Unique<Window> m_window;
		ImguiLayer* m_imguiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
	private:
		static Application* s_instance;
	};

	//defined in client
	Application* CreateApplication();

}
