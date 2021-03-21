#pragma once
#include "TituEngine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace TituEngine
{
	class OpenGLContext : public GraphicsContext
	{
	public:

		OpenGLContext(GLFWwindow* wHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_wHandle;
	};
}
