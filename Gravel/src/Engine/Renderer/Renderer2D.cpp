#include "grpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderInstruction.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Gravel {

	struct RendererData
	{
		Shared<VertexArray> VertexArray;
		Shared<Shader> Shader;
		Shared<Texture2D> WhiteTexture;
	};

	static RendererData* s_data;

	void Renderer2D::Init()
	{
		s_data = new RendererData();
		s_data->VertexArray = VertexArray::Create();

		float vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Shared<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		BufferLayout layout = {

			{ AttributeType::Float3, "a_position" },
			{ AttributeType::Float2, "a_textureCoordinates" },
		};

		// set layout vefore adding buffer to array!
		vertexBuffer->SetLayout(layout);
		s_data->VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[6] = { 0,1,2, 2, 3, 0 };

		Shared<IndexBuffer> indexBuffer;

		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_data->VertexArray->SetIndexBuffer(indexBuffer);

		s_data->WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_data->Shader = Shader::Create("res/shaders/texture2D.glsl");
		s_data->Shader->Bind();
		s_data->Shader->SetInt("u_texture", 0);
	}

	void Renderer2D::ShutDown()
	{
		delete s_data;
	}

	void Renderer2D::StartScene(const OrthographicCamera& camera)
	{
		s_data->Shader->Bind();
		s_data->Shader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_data->Shader->SetFloat4("u_color", color);
		s_data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * //rotation//
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_data->Shader->SetMat4("u_transform", transform);

		s_data->VertexArray->Bind();
		RenderInstruction::Draw(s_data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture)
	{
		s_data->Shader->SetFloat4("u_color", glm::vec4(1.0f));

		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * //rotation//
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->Shader->SetMat4("u_transform", transform);


		s_data->VertexArray->Bind();
		RenderInstruction::Draw(s_data->VertexArray);
	}
}
