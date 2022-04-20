#pragma once

#include "Engine/Core.h"

namespace Gravel {

	class GRAVEL_API Input
	{
	public:
		static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImp(keycode); }
	protected:
		virtual bool isKeyPressedImp(int keycode) = 0;
	private:
		static Input* s_instance;
	};
}