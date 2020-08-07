#pragma once

#include "pch.h"
#include "Engine/EventsManager/Events/EventBase.h"


namespace Engine {


	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "My Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		virtual ~Window() = default;
		
		virtual void Close() = 0;
		virtual void Init() = 0;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual bool IsRunning() const = 0;
		virtual void SetRunning(bool value) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};














}
