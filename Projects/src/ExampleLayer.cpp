#include "ExampleLayer.h"
#include <Imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer()
	: Layer("Example"), m_cameraController(1280.0f/720.0f, true)
{
	m_vertexArray = Gravel::VertexArray::Create();

	float vertices[7 * 3] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	Gravel::Shared<Gravel::VertexBuffer> vertexBuffer;
	vertexBuffer = Gravel::VertexBuffer::Create(vertices, sizeof(vertices));

	Gravel::BufferLayout layout = {

		{ Gravel::AttributeType::Float3, "a_position" },
		{ Gravel::AttributeType::Float4, "a_color" }
	};

	// set layout vefore adding buffer to array!
	vertexBuffer->SetLayout(layout);
	m_vertexArray->AddVertexBuffer(vertexBuffer);


	unsigned int indices[3] = { 0,1,2 };

	Gravel::Shared<Gravel::IndexBuffer> indexBuffer;
	indexBuffer = Gravel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_vertexArray->SetIndexBuffer(indexBuffer);

	m_squareVAO = Gravel::VertexArray::Create();


	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	Gravel::Shared<Gravel::VertexBuffer> squareVBO;
	squareVBO = Gravel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVBO->SetLayout({
		{ Gravel::AttributeType::Float3, "a_position" },
		{ Gravel::AttributeType::Float2, "a_textureCoordinates" }
		});
	m_squareVAO->AddVertexBuffer(squareVBO);

	unsigned int squareIndices[6] = { 0,1,2, 2, 3, 0 };
	Gravel::Shared<Gravel::IndexBuffer> squareIBO;
	squareIBO = Gravel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	m_squareVAO->SetIndexBuffer(squareIBO);

	m_flatColorShader = Gravel::Shader::Create("res/shaders/flatColor3f.glsl");

	auto textureShader = m_shaderLibrary.Load("res/shaders/texture2D.glsl");
	m_kanariTexture = Gravel::Texture2D::Create("res/textures/panda.png");

	textureShader->Bind();
	textureShader->SetInt("u_texture", 0);

}

void ExampleLayer::OnAttach()
{

}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Gravel::Timestep deltaTime)
{
	m_cameraController.OnUpdate(deltaTime);

	Gravel::RenderInstruction::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Gravel::RenderInstruction::Clear();

	Gravel::Renderer::StartScene(m_cameraController.GetCamera());

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_flatColorShader->Bind();
	m_flatColorShader->SetFloat3("u_color", m_squareColor);


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

void ExampleLayer::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Material color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Gravel::Event& event)
{
	m_cameraController.OnEvent(event);
}

