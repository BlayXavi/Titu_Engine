#include "tepch.h"
#include "Input.h"

#include "TituEngine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace TituEngine
{
	float Input::m_MouseX = 0.0f;
	float Input::m_MouseY = 0.0f;
	float Input::m_MouseXLastFrame = 0.0f;
	float Input::m_MouseYLastFrame = 0.0f;
	float Input::m_MouseDeltaX = 0.0f;
	float Input::m_MouseDeltaY = 0.0f;
	float Input::m_ScrollDelta = 0.0f;
	bool Input::m_ScrollDeltaDirty = false;

	void Input::OnMouseScrollEvent(MouseScrolledEvent& e)
	{
		m_ScrollDelta += e.GetYOffset();
		m_ScrollDeltaDirty = true;
	}

#ifdef TE_PLATFORM_WINDOWS
	GLFWwindow* m_MainWindow = nullptr;

	bool Input::IsKeyPressed(int keycode)
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(m_MainWindow, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsButtonMousePressed(int button)
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(m_MainWindow, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsAnyButtonPressed()
	{
		return IsButtonMousePressed(0) || IsButtonMousePressed(1) || IsButtonMousePressed(2);
	}

	bool Input::IsAnyButtonPressed(int& mouse)
	{
		if (IsButtonMousePressed(0))
		{
			mouse = 0;
			return true;
		}
		if (IsButtonMousePressed(1))
		{
			mouse = 1;
			return true;
		}
		if (IsButtonMousePressed(2))
		{
			mouse = 2;
			return true;
		}
		return false;
	}

	void Input::UpdateMouse()
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		m_MouseXLastFrame = m_MouseX;
		m_MouseYLastFrame = m_MouseY;

		double xpos, ypos;
		glfwGetCursorPos(m_MainWindow, &xpos, &ypos);

		m_MouseX = (float)xpos;
		m_MouseY = (float)ypos;

		m_MouseDeltaX = m_MouseXLastFrame - m_MouseX;
		m_MouseDeltaY = m_MouseYLastFrame - m_MouseY;
	}

	void Input::PostUpdateMouse()
	{
		if (m_ScrollDeltaDirty)
		{
			m_ScrollDelta = 0.0f;
			m_ScrollDeltaDirty = false;
		}
	}

#else

#error [Input Module] Platform not supported. 

#endif
}