#pragma once
#include "Engine/Platform/Window/Window.h"
#include "Engine/Renderer/IRenderingContext.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLContext.h"
#include <GLFW/glfw3.h>
#include "Engine/Renderer/Shader.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Engine/Renderer/RenderObject.h"

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
		void OnMouseMoved(MouseMovedEvent e);
		void ProcessInput();

		void Draw();

		virtual bool IsRunning() const override { return !glfwWindowShouldClose(m_Window); }
		virtual void SetRunning(bool value) override { glfwSetWindowShouldClose(m_Window, !value); }
	private:
		void SetGlfwCallbacks();

	private:

		class VertexArray* m_vertexArray1;
		unsigned int m_indexBuffer;
		unsigned int m_Texture;
		float v_TheAlpha = 0.f;
		std::vector<RenderObject> RenderObjects;

		Shader* m_Shader;
		class Camera* MainCamera;

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

