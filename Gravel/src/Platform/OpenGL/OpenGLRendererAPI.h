#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Gravel {

	class OpenGLRendererAPI : public RendererAPI
	{

	public:
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Draw(const Shared<VertexArray>& vertexArray) override;
	};

}