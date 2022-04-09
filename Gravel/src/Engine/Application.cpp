#include "grpch.h"
#include "Application.h"

#include "Engine/Log.h"

//for testing
#include <glad/glad.h>

namespace Gravel {

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		s_instance = this;
		// self deleting when out of scope
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));
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
			glClearColor(0.8,0,0.8,1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

			m_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_running = false;
		return true;
	}
}