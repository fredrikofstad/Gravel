#include "grpch.h"
#include "WindowsWindow.h"

namespace Gravel {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProperties& properties)
	{
		return new WindowsWindow(properties);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		Init(properties);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProperties& properties)
	{
		m_data.Title = properties.Title;
		m_data.Width = properties.Width;
		m_data.Height = properties.Height;

		GR_CORE_INFO("Created window {0} ({1}, {2})", properties.Title, properties.Width, properties.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on shutdown
			int success = glfwInit();
			GR_CORE_ASSERT(success, "Could not initialize GLFW.");
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow((int)properties.Width, (int)properties.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_data.VSync;
	}

}