#pragma once

#include "TituEngine.h"
#include "TituEngine/Core/Core.h"
#include "TituEngine/Events/MouseEvent.h"
#include <utility>

namespace TituEngine
{
	class TE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsButtonMousePressed(int keycode);

		static float MouseDeltaX() { return m_MouseDeltaX; }
		static float MouseDeltaY() { return m_MouseDeltaY; }

		static float GetMouseX() { return m_MouseX; }
		static float GetMouseY() { return m_MouseY; }

		static std::pair<float, float> GetMousePosition() { return std::pair<float, float>(m_MouseX, m_MouseY); }
		static std::pair<float, float> GetMouseDeltaPosition() { return std::pair<float, float>(m_MouseDeltaX, m_MouseDeltaY); }

		static float GetScrollDelta() { return m_ScrollDelta; }

	private:
		friend TituEngine::Application;
		static void UpdateMouse();
		static void PostUpdateMouse();
		static void OnMouseScrollEvent(MouseScrolledEvent& e);
	private:
		static float m_MouseX;
		static float m_MouseY;
		static float m_MouseXLastFrame;
		static float m_MouseYLastFrame;
		static float m_MouseDeltaX;
		static float m_MouseDeltaY;

		static float m_ScrollDelta;
		static bool m_ScrollDeltaDirty;
	};
}
