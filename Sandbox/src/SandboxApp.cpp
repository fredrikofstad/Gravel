#include <Gravel.h>
#include <Imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Gravel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f), m_squarePosition(0.0f)
	{
		m_vertexArray.reset(Gravel::VertexArray::Create());

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Gravel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Gravel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Gravel::BufferLayout layout = {

			{ Gravel::AttributeType::Float3, "position" },
			{ Gravel::AttributeType::Float4, "color" }
		};

		// set layout vefore adding buffer to array!
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indices[3] = { 0,1,2 };

		std::shared_ptr<Gravel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Gravel::IndexBuffer::Create(indices, std::size(indices)));
		m_vertexArray->SetIndexBuffer(indexBuffer);


		m_squareVAO.reset(Gravel::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f

		};

		std::shared_ptr<Gravel::VertexBuffer> squareVBO;
		squareVBO.reset(Gravel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVBO->SetLayout({
			{ Gravel::AttributeType::Float3, "position" },
			});
		m_squareVAO->AddVertexBuffer(squareVBO);

		unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
		std::shared_ptr<Gravel::IndexBuffer> squareIBO;
		squareIBO.reset(Gravel::IndexBuffer::Create(squareIndices, std::size(squareIndices)));

		m_squareVAO->SetIndexBuffer(squareIBO);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 color;
			
			uniform mat4 viewProjection;
			uniform mat4 transform;

			out vec3 outPosition;
			out vec4 outColor;

			void main()
			{
				gl_Position = viewProjection * transform * vec4(position, 1.0);
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
				color = vec4(outPosition * 0.5 + 0.5, 1);
				color =  outColor;
			};
		)";

		m_shader.reset(Gravel::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 position;

			uniform mat4 viewProjection;
			uniform mat4 transform;


			out vec3 outPosition;

			void main()
			{
				gl_Position = viewProjection * transform * vec4(position, 1.0);
				outPosition = position;
			};
		)";

		std::string flatColorFragmentSource = R"(
			#version 330 core

			in vec3 outPosition;
			uniform vec3 u_color;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(u_color, 1.0);
			};
		)";

		m_flatColorShader.reset(Gravel::Shader::Create(flatColorVertexSource, flatColorFragmentSource));


		//m_shader.reset(new Shader("res/shaders/Basic.shader"));
	}

	void OnUpdate(Gravel::Timestep deltaTime) override
	{

		if (Gravel::Input::isKeyPressed(GR_KEY_LEFT))
			m_cameraPosition.x -= m_cameraMoveSpeed * deltaTime;

		else if (Gravel::Input::isKeyPressed(GR_KEY_RIGHT))
			m_cameraPosition.x += m_cameraMoveSpeed * deltaTime;

		if (Gravel::Input::isKeyPressed(GR_KEY_UP))
			m_cameraPosition.y += m_cameraMoveSpeed * deltaTime;

		else if (Gravel::Input::isKeyPressed(GR_KEY_DOWN))
			m_cameraPosition.y -= m_cameraMoveSpeed * deltaTime;

		if (Gravel::Input::isKeyPressed(GR_KEY_A))
			m_cameraRotation += m_cameraRotationSpeed * deltaTime;

		else if (Gravel::Input::isKeyPressed(GR_KEY_D))
			m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
		/*
		if (Gravel::Input::isKeyPressed(GR_KEY_J))
			m_squarePosition.x -= 5 * deltaTime;

		else if (Gravel::Input::isKeyPressed(GR_KEY_L))
			m_squarePosition.x += 5 * deltaTime;

		if (Gravel::Input::isKeyPressed(GR_KEY_I))
			m_squarePosition.y += 5 * deltaTime;

		else if (Gravel::Input::isKeyPressed(GR_KEY_K))
			m_squarePosition.y -= 5 * deltaTime;

			*/

		Gravel::RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gravel::RenderInstruction::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);

		Gravel::Renderer::StartScene(m_camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Gravel::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<Gravel::OpenGLShader>(m_flatColorShader)->SetUniformFloat3("u_color", m_squareColor);


		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Gravel::Renderer::Add(m_flatColorShader, m_squareVAO, transform);
			}
		}

		Gravel::Renderer::Add(m_shader, m_vertexArray);

		Gravel::Renderer::EndScene();
	}

	void OnImguiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Material color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

	void OnEvent(Gravel::Event& event) override
	{

	}

private:
	std::shared_ptr<Gravel::VertexArray> m_vertexArray;
	std::shared_ptr<Gravel::Shader> m_shader;

	std::shared_ptr<Gravel::VertexArray> m_squareVAO;
	std::shared_ptr<Gravel::Shader> m_flatColorShader;

	Gravel::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;

	float m_cameraRotation = 0.0f;

	float m_cameraMoveSpeed = 1.0f;
	float m_cameraRotationSpeed = 10.0f;

	glm::vec3 m_squarePosition;
	glm::vec3 m_squareColor = { 0.3, 0.6, 0.34 };



};

class Sandbox : public Gravel::Application
{
public:
	Sandbox()
	{
		AddLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};


Gravel::Application* Gravel::CreateApplication()
{
	return new Sandbox();
}
