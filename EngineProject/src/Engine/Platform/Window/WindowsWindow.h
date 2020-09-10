#pragma once
#include "Engine/Platform/Window/Window.h"
#include "Engine/Renderer/IRenderingContext.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include "Engine/Renderer/Shader.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include <glm/gtc/matrix_transform.hpp>

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

		class VertexArray* m_vertexArray1;
		class VertexArray* m_vertexArray2;
		unsigned int m_indexBuffer;
		unsigned int m_Texture;
		unsigned int m_Texture2;
		float v_TheAlpha = 0.f;
		glm::mat4 TheMatrixxx;



		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_Shader2;



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

