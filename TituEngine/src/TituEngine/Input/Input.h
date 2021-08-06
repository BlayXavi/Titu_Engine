#pragma once

#include "TituEngine/Core/Core.h"
struct GLFWwindow;

#include <utility>

namespace TituEngine
{
#define INPUT(x) InputPlatform::s_Instance->##x

	class TE_API InputPlatform
	{
	protected:
		inline virtual bool IsKeyPressedImpl(int keycode) = 0;
		inline virtual bool IsButtonMousePressedImpl(int keycode) = 0;
		inline virtual float GetMouseXImpl() = 0;
		inline virtual float GetMouseYImpl() = 0;
		inline virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		friend class InputBridge;
		static InputPlatform* s_Instance;
	};

#ifdef TE_PLATFORM_WINDOWS

	class TE_API InputPlatformWindows : public InputPlatform
	{
		GLFWwindow* m_MainWindow = nullptr;

		~InputPlatformWindows()
		{
			m_MainWindow = nullptr; //input must not destroy this pointer
		}

		inline virtual bool IsKeyPressedImpl(int keycode) override;
		inline virtual bool IsButtonMousePressedImpl(int button) override;
		inline virtual float GetMouseXImpl() override;
		inline virtual float GetMouseYImpl() override;
		inline virtual std::pair<float, float> GetMousePositionImpl() override;
	};
#else
#error Platform Not Supported Context_Input
#endif

	class TE_API InputBridge
	{
	public:
		static bool IsKeyPressed(int keycode) { return INPUT(IsKeyPressedImpl(keycode)); }
		static bool IsButtonMousePressed(int button) { return INPUT(IsButtonMousePressedImpl(button)); }
		static float GetMouseX() { return INPUT(GetMouseXImpl()); }
		static float GetMouseY() { return INPUT(GetMouseYImpl()); }
		static std::pair<float, float> GetMousePosition() { return INPUT(GetMousePositionImpl()); }
	};
}
