#include "tepch.h"
#include "Input.h"

#include "TituEngine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace TituEngine
{
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

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}

	inline std::pair<float, float> Input::GetMousePosition()
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(m_MainWindow, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

#else

#error [Input Module] Platform not supported. 

#endif
}