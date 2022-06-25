#include "grpch.h"
#include "Application.h"

#include "Engine/Core/Log.h"

#include "Engine/Renderer/Renderer.h"
#include "Input.h"

#include <GLFW/glfw3.h>

namespace Gravel {


	Application* Application::s_instance = nullptr;


	Application::Application()
	{
		GR_PROFILE_FUNCTION();


		GR_CORE_ASSERT(!s_instance, "Instance of application already exits.")
		s_instance = this;

		m_window = Unique<Window>(Window::Create());
		m_window->SetEventCallback(GR_BIND_EVENT(Application::OnEvent));

		Renderer::Init();

		m_imguiLayer = new ImguiLayer();
		AddOverlay(m_imguiLayer);

	}

	Application::~Application()
	{
		GR_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::AddLayer(Layer* layer)
	{
		GR_PROFILE_FUNCTION();

		m_layerStack.AddLayer(layer);
		layer->OnAttach();
	}

	void Application::AddOverlay(Layer* layer)
	{
		GR_PROFILE_FUNCTION();

		m_layerStack.AddOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_running = false;
	}


	void Application::OnEvent(Event& event)
	{
		GR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(GR_BIND_EVENT(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(GR_BIND_EVENT(Application::OnWindowResize));


		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			if (event.Handled)
				break;
			(*--it)->OnEvent(event);
		}
	}

	void Application::Run()
	{
		GR_PROFILE_FUNCTION();

		while (m_running) 
		{
			GR_PROFILE_SCOPE("Run loop")
			//consider getting time from platform
			float time = (float)glfwGetTime();
			Timestep deltaTime = time - m_lastFrameTime;
			m_lastFrameTime = time;
			
			if (!m_minimized)
			{
				GR_PROFILE_SCOPE("Layers OnUpdate")
				for (Layer* layer : m_layerStack)
					layer->OnUpdate(deltaTime);
			}

			//moved to render thread in future
			m_imguiLayer->Start();
			{
				GR_PROFILE_SCOPE("Layers ImguiRender")
				for (Layer* layer : m_layerStack)
					layer->OnImguiRender();
				m_imguiLayer->End();
			}
			m_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) 
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		GR_PROFILE_FUNCTION()

		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		
		m_minimized = false;

		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		
		return false;
	}

}