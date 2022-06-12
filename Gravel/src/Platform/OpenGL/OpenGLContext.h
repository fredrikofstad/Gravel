#pragma once

#include "Engine/Renderer/RendererContext.h"

struct GLFWwindow;

namespace Gravel {

	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* window);
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_window;
	};

}