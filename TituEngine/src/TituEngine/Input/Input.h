#pragma once

#include "TituEngine.h"
#include "TituEngine/Core/Core.h"
#include <utility>

namespace TituEngine
{
	class TE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsButtonMousePressed(int keycode);

		static float MouseDeltaX();
		static float MouseDeltaY();

		static float GetMouseX();
		static float GetMouseY();

		static std::pair<float, float> GetMousePosition();
		static std::pair<float, float> GetMouseDeltaPosition();

	private: 
		friend TituEngine::Application;
		static void UpdateMouse();

	private:
		static float m_MouseX;
		static float m_MouseY;
		static float m_MouseXLastFrame;
		static float m_MouseYLastFrame;
		static float m_MouseDeltaX;
		static float m_MouseDeltaY;
	};
}
