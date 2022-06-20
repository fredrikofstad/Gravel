#pragma once

#include "Engine/Camera/OrthographicCamera.h"
#include "Texture.h"

namespace Gravel {


	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void StartScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture2D>& texture);
			
	};


}
