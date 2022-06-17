#include "grpch.h"
#include "Renderer.h"

namespace Gravel {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

	void Renderer::StartScene(OrthographicCamera& camera)
	{
		s_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Add(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniformMat4("viewProjection", s_sceneData->ViewProjectionMatrix);
		shader->SetUniformMat4("transform", transform);

		vertexArray->Bind();
		RenderInstruction::Draw(vertexArray);
	}

}