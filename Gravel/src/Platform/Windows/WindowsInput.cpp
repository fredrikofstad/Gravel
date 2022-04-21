#include "grpch.h"
#include "WinddowsInput.h"

#include <GLFW/glfw3.h>
#include "Engine/Application.h"

namespace Gravel {

	bool WindowsInput::IsKeyPressed(int keycode)
	{

		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

}