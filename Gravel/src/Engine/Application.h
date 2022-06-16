#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/LayerStack.h"
#include "Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Imgui/ImguiLayer.h"

#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/Shader.h"


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

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<Shader> m_shader;

		std::shared_ptr<VertexArray> m_squareVAO;
		std::shared_ptr<Shader> m_shader2;


		static Application* s_instance;
	};

	//defined in client
	Application* CreateApplication();

}
