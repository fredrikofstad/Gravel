#include "grpch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderInstruction.h"
#include "VertexArray.h"
#include "Shader.h"

namespace Gravel {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCooridates;
		float TextureIndex;
		float Tiling;

	};

	struct RendererData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVerticies = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;


		uint32_t IndexCount = 0;
		QuadVertex* BufferBase = nullptr;
		QuadVertex* BufferPointer = nullptr;

		std::array<Shared<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		Shared<VertexArray> VertexArray;
		Shared<VertexBuffer> VertexBuffer;
		Shared<Shader> Shader;
		Shared<Texture2D> WhiteTexture;
		glm::vec4 VertexPositions[4];

		Renderer2D::Statistics Statistics;
	};

	static RendererData s_data;

	void Renderer2D::Init()
	{
		GR_PROFILE_FUNCTION();

		s_data.VertexArray = VertexArray::Create();

		s_data.VertexBuffer = VertexBuffer::Create(s_data.MaxVerticies * sizeof(QuadVertex));

		BufferLayout layout = {

			{ AttributeType::Float3, "a_position" },
			{ AttributeType::Float4, "a_color" },
			{ AttributeType::Float2, "a_textureCoordinates" },
			{ AttributeType::Float, "a_textureIndex" },
			{ AttributeType::Float, "a_tiling" },
		};
		// set layout vefore adding buffer to array!
		s_data.VertexBuffer->SetLayout(layout);

		s_data.VertexArray->AddVertexBuffer(s_data.VertexBuffer);

		s_data.BufferBase = new QuadVertex[s_data.MaxVerticies];

		uint32_t* indices = new uint32_t[s_data.MaxIndices];

		//setting indices for quads (6) for every quad
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4; // quads are four vertices
		}

		Shared<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_data.MaxIndices);
		s_data.VertexArray->SetIndexBuffer(indexBuffer);

		delete[] indices;

		s_data.WhiteTexture = Texture2D::Create(1,1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_data.MaxTextureSlots];
		for (int32_t i = 0; i < s_data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_data.Shader = Shader::Create("res/shaders/switch.glsl");
		s_data.Shader->Bind();
		s_data.Shader->SetIntArray("u_textures", samplers, s_data.MaxTextureSlots);

		//set all texture slots to 0
		s_data.TextureSlots[0] = s_data.WhiteTexture;

		s_data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		GR_PROFILE_FUNCTION();

		delete[] s_data.BufferBase;
	}

	void Renderer2D::StartScene(const OrthographicCamera& camera)
	{
		GR_PROFILE_FUNCTION();

		s_data.Shader->Bind();
		s_data.Shader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());
		StartBatch();
	}

	void Renderer2D::StartScene(const Camera& camera, const glm::mat4& transform)
	{
		GR_PROFILE_FUNCTION();

		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_data.Shader->Bind();
		s_data.Shader->SetMat4("u_viewProjection", viewProjection);

		s_data.IndexCount = 0;
		s_data.BufferPointer = s_data.BufferBase;

		s_data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		GR_PROFILE_FUNCTION();
		uint32_t size = (uint32_t)((uint8_t*)s_data.BufferPointer - (uint8_t*)s_data.BufferBase);
		s_data.VertexBuffer->SetData(s_data.BufferBase, size);
		Flush();
	}

	void Renderer2D::Flush()
	{
		if (s_data.IndexCount == 0)
			return;

		for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
			s_data.TextureSlots[i]->Bind(i);

		RenderInstruction::Draw(s_data.VertexArray, s_data.IndexCount);
		s_data.Statistics.DrawCalls++;
	}

	void Renderer2D::StartBatch()
	{
		// keeping track of wherer we are in buffer
		s_data.IndexCount = 0;
		s_data.BufferPointer = s_data.BufferBase;

		s_data.TextureSlotIndex = 1;
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GR_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		constexpr glm::vec2 textureCoordinates[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			//get pointer and then dereference it
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		DrawQuadBody(transform, textureIndex, tiling, tintColor, textureCoordinates);


	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture>& subTexture, float tiling, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture>& subTexture, float tiling, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		constexpr size_t vertexCount = 4;
		const glm::vec2* textureCoordinates = subTexture->GetCoordinates();
		const Shared<Texture2D> texture = subTexture->GetTexture();

		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			//get pointer and then dereference it
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		DrawQuadBody(transform, textureIndex, tiling, tintColor, textureCoordinates);

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}


		const float textureIndex = 0.0f;
		const float tiling = 1.0f;
		constexpr glm::vec2 textureCoordinates[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		constexpr size_t vertexCount = 4;

		//pushing data into buffer
		for (size_t i = 0; i < vertexCount; i++)
		{
			s_data.BufferPointer->Position = transform * s_data.VertexPositions[i];
			s_data.BufferPointer->Color = color;
			s_data.BufferPointer->TextureCooridates = textureCoordinates[i];
			s_data.BufferPointer->TextureIndex = textureIndex;
			s_data.BufferPointer->Tiling = tiling;
			s_data.BufferPointer++;
		}

		s_data.IndexCount += 6;

		s_data.Statistics.QuadCount++;

		//DrawQuadBody(transform, 0.0f, tiling, color, textureCoordinates);

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		constexpr glm::vec2 textureCoordinates[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}

		glm::vec4 color = tintColor;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			//get pointer and then dereference it
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		DrawQuadBody(transform, textureIndex, tiling, tintColor, textureCoordinates);

	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, float tiling, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		constexpr glm::vec2 textureCoordinates[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		glm::vec4 color = tintColor;

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			//get pointer and then dereference it
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		DrawQuadBody(transform, textureIndex, tiling, tintColor, textureCoordinates);

	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture>& subTexture, float tiling, const glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x, position.y, 0.0f }, size, rotation, subTexture, tiling, tintColor);
	}

	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture>& subTexture, float tiling, const glm::vec4& tintColor)
	{
		GR_PROFILE_FUNCTION();

		constexpr size_t vertexCount = 4;
		const glm::vec2* textureCoordinates = subTexture->GetCoordinates();
		const Shared<Texture2D> texture = subTexture->GetTexture();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		if (s_data.IndexCount >= s_data.MaxIndices)
		{
			EndScene();
			StartBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
		{
			//get pointer and then dereference it
			if (*s_data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		DrawQuadBody(transform, textureIndex, tiling, tintColor, textureCoordinates);

	}

	Renderer2D::Statistics Renderer2D::GetStatistics()
	{
		return s_data.Statistics;
	}

	void Renderer2D::ResetStatistics()
	{
		memset(&s_data.Statistics, 0, sizeof(Renderer2D::Statistics));
	}


	void Renderer2D::DrawQuadBody(const glm::mat4 transform, float textureIndex, float tiling, const glm::vec4& tintColor, const glm::vec2 textureCoordinates[])
	{
		GR_PROFILE_FUNCTION();

		constexpr size_t vertexCount = 4;

		//pushing data into buffer
		for (size_t i = 0; i < vertexCount; i++)
		{
			s_data.BufferPointer->Position = transform * s_data.VertexPositions[i];
			s_data.BufferPointer->Color = tintColor;
			s_data.BufferPointer->TextureCooridates = textureCoordinates[i];
			s_data.BufferPointer->TextureIndex = textureIndex;
			s_data.BufferPointer->Tiling = tiling;
			s_data.BufferPointer++;
		}

		s_data.IndexCount += 6;

		s_data.Statistics.QuadCount++;
	}

}
