#include "grpch.h"
#include "Application.h"

#include "Engine/Log.h"

//for testing
#include <glad/glad.h>

#include "Input.h"
//#include "glm/glm.hpp"

namespace Gravel {

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		s_instance = this;
		// self deleting when out of scope
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));

		m_imguiLayer = new ImguiLayer();
		AddOverlay(m_imguiLayer);

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			out vec3 outPosition;

			void main()
			{
				gl_Position = vec4(position, 1.0);
				outPosition = position;
			};
		)";

		std::string fragmentSource = R"(
			#version 330 core

			in vec3 outPosition;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(outPosition * 0.5 + 0.5, 1.0);
			};
		)";


		m_shader.reset(new Shader(vertexSource, fragmentSource));

		//m_shader.reset(new Shader("res/shaders/Basic.shader"));
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
			glClearColor(0.1f,0.1f,0.1f,1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->Bind();

			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack)
				layer->OnUpdate();

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