#pragma once

#include "Engine/Core/Input.h"

namespace Gravel {

	class WindowsInput : public Input
	{
	protected:
		virtual bool isKeyPressedImp(int keycode) override;

		virtual bool isMouseButtonPressedImp(int button) override;

		virtual std::pair<float, float> GetMousePositionImp() override;
		virtual float GetMouseXImp() override;
		virtual float GetMouseYImp() override;

	};

}