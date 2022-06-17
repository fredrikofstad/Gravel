#include "grpch.h"
#include "Application.h"

#include "Engine/Log.h"

#include "Engine/Renderer/Renderer.h"
#include "Input.h"

#include <GLFW/glfw3.h>

namespace Gravel {

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;


	Application::Application()
	{
		GR_CORE_ASSERT(!s_instance, "Instance of application already exits.")
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));

		m_imguiLayer = new ImguiLayer();
		AddOverlay(m_imguiLayer);

	}

	Application::~Application()
	{
	}

	void Application::AddLayer(Layer* layer)
	{
		m_layerStack.AddLayer(layer);
		layer->OnAttach();
	}

	void Application::AddOverlay(Layer* layer)
	{
		m_layerStack.AddOverlay(layer);
		layer->OnAttach();
	}


	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT(Application::OnWindowClose));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_running) 
		{
			//consider getting time from platform
			float time = (float)glfwGetTime();
			Timestep deltaTime = time - m_lastFrameTime;
			m_lastFrameTime = time;
			
			for (Layer* layer : m_layerStack)
				layer->OnUpdate(deltaTime);

			//moved to render thread in future
			m_imguiLayer->Start();
			for (Layer* layer : m_layerStack)
				layer->OnImguiRender();
			m_imguiLayer->End();

			m_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_running = false;
		return true;
	}
}