#pragma once
#include "Engine/Window/Window.h"
#include "GLFW/glfw3.h"

namespace Engine {


	class WindowsWindow : public Engine::Window
	{

	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }

		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		//virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.Callback = callback; }

		virtual void SetVSync(bool enabled) override;

		virtual bool IsVSync() const override { return m_Data.VSync; }

		virtual void* GetNativeWindow() const override { return m_Window; }

		virtual void Init() override;
	private:
		void SetGlfwCallbacks();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync = false;

			//EventCallbackFn Callback;
		};

		WindowData m_Data;
	};



}

