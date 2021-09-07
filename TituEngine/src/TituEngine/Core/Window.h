#pragma once

#include "tepch.h"

#include "Core.h"
#include "../Events/Event.h"

namespace TituEngine
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Titu Engine", uint32_t width = 1920, uint32_t height = 1080) :
			Title(title), Width(width), Height(height) {}
	};

	//Interface to be implemented for each desktop platform
	class TE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}