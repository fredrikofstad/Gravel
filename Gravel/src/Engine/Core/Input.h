#pragma once

#include <glm/glm.hpp>

#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Gravel {

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode keycode);
		static bool isMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();
	};
}