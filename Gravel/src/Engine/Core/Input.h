#pragma once

#include "Engine/Core/Core.h"

namespace Gravel {

	class GRAVEL_API Input
	{
	public:
		inline static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImp(keycode); }
		inline static bool isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressedImp(button); }
		inline static float GetMouseX() { return s_instance->GetMouseXImp(); }
		inline static float GetMouseY() { return s_instance->GetMouseYImp(); }
		inline static std::pair<float,float> GetMousePosition() { return s_instance->GetMousePositionImp(); }

	protected:
		virtual bool isKeyPressedImp(int keycode) = 0;

		virtual bool isMouseButtonPressedImp(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImp() = 0;

		virtual float GetMouseXImp() = 0;
		virtual float GetMouseYImp() = 0;

	private:
		static Input* s_instance;
	};
}