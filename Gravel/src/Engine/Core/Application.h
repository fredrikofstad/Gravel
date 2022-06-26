#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Imgui/ImguiLayer.h"


int main(int argc, char** argv);

namespace Gravel {

	class Application
	{
	public:
		Application(const std::string& name = "Gravel Application");
		virtual ~Application();

		void OnEvent(Event& event);

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }

		void Close();
		void Run();

		inline static Application& Get() { return *s_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		Unique<Window> m_window;
		ImguiLayer* m_imguiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
	private:
		static Application* s_instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication();

}
