#include "grpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Gravel{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_window(window)
	{
		GR_CORE_ASSERT(window, "Window is null!")
	}

	void OpenGLContext::Init()
	{
		GR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GR_CORE_ASSERT(status, "Failed to initialize Glad.");

		GR_CORE_INFO("--- OpenGL ---");
		GR_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		GR_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
		GR_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		GR_PROFILE_FUNCTION();

		glfwSwapBuffers(m_window);
	}


}