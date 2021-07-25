#include "tepch.h"
#include "Input.h"

#include "TituEngine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace TituEngine
{
#ifdef TE_PLATFORM_WINDOWS
	InputPlatform* InputPlatform::s_Instance = new InputPlatformWindows;

	bool InputPlatformWindows::IsKeyPressedImpl(int keycode)
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(m_MainWindow, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool InputPlatformWindows::IsButtonMousePressedImpl(int button)
	{
		if (m_MainWindow == nullptr)
			m_MainWindow = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(m_MainWindow, button);
		return false;
	}

	float InputPlatformWindows::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return (float)x;
	}

	float InputPlatformWindows::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

	inline std::pair<float, float> InputPlatformWindows::GetMousePositionImpl()
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