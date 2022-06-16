#pragma once

#include "RenderInstruction.h"

namespace Gravel {

	class Renderer
	{
	public:
		static void StartScene();
		static void EndScene();

		static void Add(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}