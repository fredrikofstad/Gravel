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
		GR_CORE_ASSERT(!s_instance, "Instance of application already exits.")
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT(Application::OnEvent));

		m_imguiLayer = new ImguiLayer();
		AddOverlay(m_imguiLayer);

		m_vertexArray.reset(VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		BufferLayout layout = {

			{ AttributeType::Float3, "position" },
			{ AttributeType::Float4, "color" }
		};

		// set layout vefore adding buffer to array!
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indices[3] = { 0,1,2 };

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, std::size(indices)));
		m_vertexArray->SetIndexBuffer(indexBuffer);


		m_squareVAO.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f

		};

		std::shared_ptr<VertexBuffer> squareVBO;
		squareVBO.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVBO->SetLayout({
			{ AttributeType::Float3, "position" },
		});
		m_squareVAO->AddVertexBuffer(squareVBO);

		unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIBO;
		squareIBO.reset(IndexBuffer::Create(squareIndices, std::size(squareIndices)));

		m_squareVAO->SetIndexBuffer(squareIBO);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;


			out vec3 outPosition;
			out vec4 outColor;

			void main()
			{
				gl_Position = vec4(position, 1.0);
				outPosition = position;
				outColor = color;
			};
		)";

		std::string fragmentSource = R"(
			#version 330 core

			in vec3 outPosition;
			in vec4 outColor;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(outPosition * 0.5 + 0.5, 1.0);
				color =  outColor;
			};
		)";

		m_shader.reset(new Shader(vertexSource, fragmentSource));

		std::string vertexSource2 = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			out vec3 outPosition;

			void main()
			{
				gl_Position = vec4(position, 1.0);
				outPosition = position;
			};
		)";

		std::string fragmentSource2 = R"(
			#version 330 core

			in vec3 outPosition;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(outPosition * 0.5 + 0.5, 1.0);
			};
		)";

		m_shader2.reset(new Shader(vertexSource2, fragmentSource2));


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


			m_shader2->Bind();
			m_squareVAO->Bind();

			glDrawElements(GL_TRIANGLES, m_squareVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_shader->Bind();

			m_vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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