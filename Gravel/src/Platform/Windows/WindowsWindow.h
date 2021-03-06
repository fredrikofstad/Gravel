#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/RendererContext.h"

#include <GLFW/glfw3.h>

namespace Gravel {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.Width; }
		inline unsigned int GetHeight() const override { return m_data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window; }
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();
	private:
		GLFWwindow* m_window;
		Unique<RendererContext> m_context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_data;

	};
}

