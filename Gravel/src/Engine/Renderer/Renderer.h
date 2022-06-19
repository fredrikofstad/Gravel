#pragma once

#include "RenderInstruction.h"
#include "Engine/Camera/OrthographicCamera.h"
#include "Shader.h"

namespace Gravel {

	class Renderer
	{
	public:
		static void Init();

		static void StartScene(OrthographicCamera& camera);
		static void EndScene();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void Add(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_sceneData;
	
	};

}