#include "pch.h"
#include "WindowsWindow.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include <glad/glad.h>
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int errorType, const char* errorDesc)
	{
		CORE_CRITICAL("GLFW Error Code: {0} , {1}", errorType, errorDesc);
	}

	Window* Window::Create(const WindowProps& props /* = WindowProps() */)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
	}
	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void WindowsWindow::Close()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}


	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Data.VSync = enabled;
		if (enabled)
		{
			glfwSwapInterval(1);//limit fps to monitor refresh rate
		}
		else
		{
			glfwSwapInterval(0);
		}
	}

	void WindowsWindow::Init()
	{
		CORE_INFO("Creating window {0} ({1},{2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			ENGINE_CORE_ASSERT(success, "Could not initiaze GLFW");


			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);


		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		ENGINE_CORE_ASSERT(m_Window, "Could not create window");

		m_RenderingContext = new OpenGLContext(m_Window);

		m_RenderingContext->Init();
		glViewport(0, 0, m_Data.Width, m_Data.Height);


		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		SetGlfwCallbacks();

		DoOpenGlStuff();
	}
	void WindowsWindow::OnUpdate()
	{
		m_Shader->Bind();

		m_vertexArray1->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);


		m_RenderingContext->SwapBuffers();
		glfwPollEvents();
	}


	void WindowsWindow::DoOpenGlStuff()
	{

		//vertex stuff

		//constexpr float vertices[] = {
		//  0.5f, 0.5f, 0.0f,					//rectangle
		// 0.5f, -0.5f, 0.0f,
		//-0.5f, -0.5f, 0.0f,
		// -0.5f, 0.5f, 0.0f
		//};

// 		constexpr float vertices[] = {
// 		 -0.5f, 0.5f, 0.0f,					//triangle
// 		 0.0f, -0.5f, 0.0f,
// 		-1.0f, -0.5f, 0.0f,
// 		};

// 		constexpr float vertices[] = {
// 			0.5f,0.5f,0.0f,
// 		   1.0f,-0.5f,0.0f,
// 		 0.0f, -0.5f, 0.0f,
// 
// 		 0.0f, -0.5f, 0.0f,
// 		-1.0f, -0.5f, 0.0f,
// 		-0.5f, 0.5f, 0.0f,					// 2 triangles clockwise
// 		};

// 		constexpr float vertices[] = {
// 		 0.0f, -0.5f, 0.0f,
// 		-1.0f, -0.5f, 0.0f,
// 		 -0.5f, 0.5f, 0.0f,					// 2 triangles
// 		   1.0f,-0.5f,0.0f,
// 		 0.0f, -0.5f, 0.0f,
// 			0.5f,0.5f,0.0f,
// 		};

		constexpr float vertices[] = {
		 -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,					//triangle and colors
		 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		VertexBuffer buffer(GL_ARRAY_BUFFER);
		buffer.Bind();
		buffer.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);


		VertexArray* VAO = new VertexArray();
		VAO->Bind();
		buffer.Bind();
		VAO->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		VAO->EnableAttribArray(0);
		VAO->SetAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO->EnableAttribArray(1);

		m_vertexArray1 = VAO;


		m_Shader = std::make_unique<Shader>("D:/C++/EngineProject/SandboxTest/Assets/Shaders/vertexShader.glsl", "D:/C++/EngineProject/SandboxTest/Assets/Shaders/fragmentShader.glsl");

	}


	void WindowsWindow::SetGlfwCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->Width = width;
				data->Height = height;

				WindowResizeEvent e = WindowResizeEvent(width, height);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);

				WindowBufferResizeEvent e = WindowBufferResizeEvent(width, height);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowCloseEvent e = WindowCloseEvent(window);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow*, int keycode, int scandCode, int action, int mods)
			{
				EventManager& instance = EventManager::GetInstance();

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e = KeyPressedEvent(keycode);
					instance.SendEvent(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e = KeyReleasedEvent(keycode);
					instance.SendEvent(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyRepeatEvent e = KeyRepeatEvent(keycode, 1);
					instance.SendEvent(e);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow*, int button, int action, int mods)
			{
				EventManager& instance = EventManager::GetInstance();
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e = MouseButtonPressedEvent(button);
					instance.SendEvent(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e = MouseButtonReleasedEvent(button);
					instance.SendEvent(e);
					break;
				}
				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow*, double x, double y)
			{
				MouseScrollEvent e = MouseScrollEvent((float)x, (float)y);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow*, double x, double y)
			{
				MouseMovedEvent e = MouseMovedEvent((float)x, (float)y);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				KeyTypedEvent e = KeyTypedEvent(keycode);
				EventManager::GetInstance().SendEvent(e);
			});
	}
}
