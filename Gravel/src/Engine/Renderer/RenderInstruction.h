#pragma once

#include "RendererAPI.h"

namespace Gravel {

	class RenderInstruction
	{
	public:
		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_rendererAPI->Clear();
		}

		inline static void Draw(const Shared<VertexArray>& vertexArray)
		{
			s_rendererAPI->Draw(vertexArray);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};

}