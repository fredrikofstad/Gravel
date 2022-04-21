#pragma once

#include "Engine/Input.h"

namespace Gravel {

	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressed(int keycode) override;
	};

}