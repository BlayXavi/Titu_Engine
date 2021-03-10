#pragma once

#include "tepch.h"

#include "Core.h"
#include "Events/Event.h"

namespace TituEngine
{
	struct WindowProps
	{
		std::string Title;
		uint Width;
		uint Height;

		WindowProps(const std::string& title = "Titu Engine", uint width = 1280, uint height = 720) :
			Title(title), Width(width), Height(height) {}
	};

	//Interface to be implemented for each desktop platform
	class TE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;
		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}