#pragma once

#include "TituEngine/Core/Core.h"
#include <utility>

namespace TituEngine
{
	class TE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsButtonMousePressed(int keycode);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};
}
