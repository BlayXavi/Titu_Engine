#pragma once

#include "GLFW/glfw3.h"
#include "TituEngine/Core/Window.h"
#include "TituEngine/Renderer/GraphicsContext.h"

namespace TituEngine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint GetWidth() const override { return m_Data.Width; }
		inline uint GetHeight() const override { return m_Data.Height; }
		inline bool IsVsync() const override { return m_Data.VSync; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint Width, Height;
			bool VSync;

			EventCallbackFn EventCallback; //m_Data.EventCallback(event) to call the event bound function
		};

		WindowData m_Data;
	};
}


