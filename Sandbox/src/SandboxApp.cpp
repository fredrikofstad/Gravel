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
		: Layer("Example"), m_cameraController(1280.0f/720.0f, true)
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

		m_flatColorShader = Gravel::Shader::Create("res/shaders/flatColor.glsl");

		auto textureShader = m_shaderLibrary.Load("res/shaders/texture2D.glsl");
		m_kanariTexture = Gravel::Texture2D::Create("res/textures/panda.png");

		std::dynamic_pointer_cast<Gravel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Gravel::OpenGLShader>(textureShader)->SetUniformInt("u_texture", 0);

	}

	void OnUpdate(Gravel::Timestep deltaTime) override
	{
		m_cameraController.OnUpdate(deltaTime);

		Gravel::RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gravel::RenderInstruction::Clear();

		Gravel::Renderer::StartScene(m_cameraController.GetCamera());

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

		auto textureShader = m_shaderLibrary.Get("texture2D");

		m_kanariTexture->Bind();
		Gravel::Renderer::Add(textureShader, m_squareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


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

	void OnEvent(Gravel::Event& e) override
	{
		m_cameraController.OnEvent(e);
	}

private:
	Gravel::ShaderLibrary m_shaderLibrary;
	Gravel::Shared<Gravel::VertexArray> m_vertexArray;
	Gravel::Shared<Gravel::Shader> m_shader;

	Gravel::Shared<Gravel::VertexArray> m_squareVAO;
	Gravel::Shared<Gravel::Shader> m_flatColorShader;

	Gravel::Shared<Gravel::Texture2D> m_kanariTexture;

	Gravel::OrthographicCameraConrtoller m_cameraController;

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
