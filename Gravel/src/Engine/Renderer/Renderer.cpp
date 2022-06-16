#include "grpch.h"
#include "Renderer.h"

namespace Gravel {

	void Renderer::StartScene()
	{

	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Add(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderInstruction::Draw(vertexArray);
	}

}