#include "tepch.h"
#include "Timestep.h"

#if TE_PLATFORM_WINDOWS
#include <GLFW/glfw3.h>
#endif

namespace TituEngine
{

	float Timestep::GetCurrentTime()

	{
#if TE_PLATFORM_WINDOWS
		return (float)glfwGetTime();
#else
#error NO SUPPORTED PLATFORM - Timestep class
#endif
	}

}