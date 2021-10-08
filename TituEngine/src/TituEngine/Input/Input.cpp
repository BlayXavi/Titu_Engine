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

	float Input::MouseDeltaX()
	{
		return m_MouseDeltaX;
	}

	float Input::MouseDeltaY()
	{
		return m_MouseDeltaY;
	}

	float Input::GetMouseX()
	{
		return m_MouseX;
	}

	float Input::GetMouseY()
	{
		return m_MouseY;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return std::pair<float, float>(m_MouseX, m_MouseY);
	}

	std::pair<float, float> Input::GetMouseDeltaPosition()
	{
		return std::pair<float, float>(m_MouseDeltaX, m_MouseDeltaY);
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

#else

#error [Input Module] Platform not supported. 

#endif
}