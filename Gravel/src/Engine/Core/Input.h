#pragma once

#include "Engine/Core/Core.h"

namespace Gravel {

	class Input
	{
	public:
		static bool isKeyPressed(int keycode);
		static bool isMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};
}