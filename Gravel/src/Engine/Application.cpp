#include "grpch.h"
#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Log.h"

//for testing
#include <GLFW/glfw3.h>

namespace Gravel {

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		// self deleting when out of scope
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& event)
	{
		GR_CORE_INFO("{0}", event);
	}

	void Application::Run()
	{
		while (m_running) 
		{
			glClearColor(0.8,0,0.8,1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}
}