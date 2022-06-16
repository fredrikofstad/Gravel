#pragma once

namespace Gravel {

	enum class RendererAPI
	{
		None, OpenGL
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetCurrentAPI() { return s_rendererAPI; }
	private:
		static RendererAPI s_rendererAPI;
	};

}