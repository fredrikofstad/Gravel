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

		inline static void Draw(const Shared<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_rendererAPI->Draw(vertexArray, indexCount);
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

	private:
		static Unique<RendererAPI> s_rendererAPI;
	};

}