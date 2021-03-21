#include "tepch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace TituEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* wHandle) : m_wHandle(wHandle)
	{
		TE_ASSERT(wHandle, "No windows instantiated!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_wHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TE_ASSERT(status, "Failed to initializee glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_wHandle);
	}
}