#include "grpch.h"
#include "Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace Gravel {


	// Todo:: abstract this into platform api
	Unique<Window> Window::Create(const WindowProperties& properties)
	{
		return MakeUnique<WindowsWindow>(properties);
	}

}