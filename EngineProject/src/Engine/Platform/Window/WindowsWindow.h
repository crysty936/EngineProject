#pragma once
#include "Engine/Platform/Window/Window.h"
#include "Engine/Renderer/IRenderingContext.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>

namespace Engine {


	class WindowsWindow : public Engine::Window
	{

	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void Close() override;

		void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; }

		virtual uint32_t GetHeight() const override { return m_Data.Height; }

		virtual void SetVSync(bool enabled) override;

		virtual bool IsVSync() const override { return m_Data.VSync; }

		virtual void* GetNativeWindow() const override { return m_Window; }

		virtual void Init() override;

		void DoOpenGlStuff();

		virtual bool IsRunning() const override { return !glfwWindowShouldClose(m_Window); }
		virtual void SetRunning(bool value) override { glfwSetWindowShouldClose(m_Window, !value); }
	private:
		void SetGlfwCallbacks();

	private:

		unsigned int m_shaderProgram;
		unsigned int m_vertexArray;
		unsigned int m_vertexBuffer;
		unsigned int m_indexBuffer;



		IRenderingContext* m_RenderingContext;
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0;
			unsigned int Height = 0;
			bool VSync = false;
		};

		WindowData m_Data;
	};



}

