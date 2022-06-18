#include <Gravel.h>
#include <Imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

class ExampleLayer : public Gravel::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f), m_cameraPosition(0.0f), m_squarePosition(0.0f)
	{
		m_vertexArray = std::make_shared<Gravel::OpenGLVertexArray>();

		float vertices[7 * 3] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
		};

		Gravel::Shared<Gravel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Gravel::VertexBuffer::Create(vertices, sizeof(vertices)));

		Gravel::BufferLayout layout = {

			{ Gravel::AttributeType::Float3, "a_position" },
			{ Gravel::AttributeType::Float4, "a_color" }
		};

		// set layout vefore adding buffer to array!
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indices[3] = { 0,1,2 };

		Gravel::Shared<Gravel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Gravel::IndexBuffer::Create(indices, std::size(indices)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_squareVAO = Gravel::VertexArray::Create();


		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Gravel::Shared<Gravel::VertexBuffer> squareVBO;
		squareVBO.reset(Gravel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVBO->SetLayout({
			{ Gravel::AttributeType::Float3, "a_position" },
			{ Gravel::AttributeType::Float2, "a_textureCoordinates" }
			});
		m_squareVAO->AddVertexBuffer(squareVBO);

		unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
		Gravel::Shared<Gravel::IndexBuffer> squareIBO;
		squareIBO.reset(Gravel::IndexBuffer::Create(squareIndices, std::size(squareIndices)));

		m_squareVAO->SetIndexBuffer(squareIBO);

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec4 a_color;
			
			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;

			out vec4 v_color;

			void main()
			{
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
				v_color = a_color;
			};
		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec4 v_color;

			void main()
			{
				color = v_color;
			};
		)";

		m_shader.reset(Gravel::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;


			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;


			out vec3 v_position;

			void main()
			{
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
				v_position = a_position;
			};
		)";

		std::string flatColorFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_color;

			void main()
			{
				color = vec4(u_color, 1.0);
			};
		)";

		m_flatColorShader.reset(Gravel::Shader::Create(flatColorVertexSource, flatColorFragmentSource));


		std::string textureVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_textureCoordinates;
			
			out vec2 v_textureCoordinates;

			uniform mat4 u_viewProjection;
			uniform mat4 u_transform;


			void main()
			{
				gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
				v_textureCoordinates = a_textureCoordinates;
			};
		)";

		std::string textureFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec2 v_textureCoordinates;

			uniform sampler2D u_texture;

			void main()
			{
				color = texture(u_texture, v_textureCoordinates);
			};
		)";

		m_textureShader.reset(Gravel::Shader::Create(textureVertexSource, textureFragmentSource));
		m_kanariTexture = Gravel::Texture2D::Create("res/textures/the_pixels.png");

		std::dynamic_pointer_cast<Gravel::OpenGLShader>(m_textureShader)->Bind();
		std::dynamic_pointer_cast<Gravel::OpenGLShader>(m_textureShader)->SetUniformInt("u_texture", 0);


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

		m_kanariTexture->Bind();
		Gravel::Renderer::Add(m_textureShader, m_squareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		//triangle
		//Gravel::Renderer::Add(m_shader, m_vertexArray);

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
	Gravel::Shared<Gravel::VertexArray> m_vertexArray;
	Gravel::Shared<Gravel::Shader> m_shader;

	Gravel::Shared<Gravel::VertexArray> m_squareVAO;
	Gravel::Shared<Gravel::Shader> m_flatColorShader, m_textureShader;

	Gravel::Shared<Gravel::Texture2D> m_kanariTexture;
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
