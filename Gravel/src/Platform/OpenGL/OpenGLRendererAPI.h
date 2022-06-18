#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Gravel {

	class OpenGLRendererAPI : public RendererAPI
	{

	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void Draw(const std::shared_ptr<VertexArray>& vertexArray) override;
	};

}