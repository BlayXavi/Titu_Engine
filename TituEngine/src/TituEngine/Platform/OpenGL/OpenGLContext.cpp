#include "tepch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace TituEngine
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         TE_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       TE_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          TE_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: TE_CORE_TRACE(message); return;
		}

		TE_ASSERT(false, "Unknown severity level!");
	}

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

#ifdef TE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_wHandle);
	}
}