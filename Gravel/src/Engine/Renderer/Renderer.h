#pragma once

#include "RenderInstruction.h"
#include "Engine/Camera/OrthographicCamera.h"
#include "Shader.h"

namespace Gravel {

	class Renderer
	{
	public:
		static void StartScene(OrthographicCamera& camera);
		static void EndScene();

		static void Add(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_sceneData;
	
	};

}