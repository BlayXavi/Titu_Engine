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

		TE_CORE_INFO("OpenGL Info:\nVENDOR: {0}\nRENDERER: {1}\nVERSION: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_wHandle);
	}
}