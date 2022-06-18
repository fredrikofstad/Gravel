#include "grpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Gravel {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderInstruction::Init();
	}

	void Renderer::StartScene(OrthographicCamera& camera)
	{
		s_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Add(const Shared<Shader>& shader, const Shared<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_viewProjection", s_sceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetUniformMat4("u_transform", transform);
		vertexArray->Bind();
		RenderInstruction::Draw(vertexArray);
	}

}