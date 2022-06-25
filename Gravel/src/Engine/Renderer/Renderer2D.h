#pragma once

#include "Engine/Camera/OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture.h"

namespace Gravel {


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void StartScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<SubTexture>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<SubTexture>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<SubTexture>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<SubTexture>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		//stats

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; };
			uint32_t GetTotalIndexCount() { return QuadCount * 6; };
		};

		static Statistics GetStatistics();
		static void ResetStatistics();

	private: 
		static void StartBatch();

		static void DrawQuadBody(const glm::vec3& position, const glm::vec2& size, float rotation, float textureIndex, float tiling, const glm::vec4& tintColor, const glm::vec2 textureCoordinates[]);
	};


}
