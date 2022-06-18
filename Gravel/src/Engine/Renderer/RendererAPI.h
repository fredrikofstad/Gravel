#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Gravel {

	class RendererAPI
	{
	public:
		enum class API
		{
			None, OpenGL
		};

	public:
		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void Draw(const Shared<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;

	};

}