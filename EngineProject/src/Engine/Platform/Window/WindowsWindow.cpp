#include "pch.h"
#include "WindowsWindow.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include <glad/glad.h>
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Core/KeyCodes.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {




	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int errorType, const char* errorDesc)
	{
		LOG_CORE_CRITICAL("GLFW Error Code: {0} , {1}", errorType, errorDesc);
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
		LOG_CORE_INFO("Creating window {0} ({1},{2})", m_Data.Title, m_Data.Width, m_Data.Height);

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
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);






		Point A(0.0f, 1.0f, 0.0f);
		Point B(-0.5f, -1.0f, 0.0f);
		Point C(0.5f, -1.0f, 0.0f);


		ThatSerpinskiRecursiveFunction(A, B, C, 5);






		m_RenderingContext->SwapBuffers();
		glfwPollEvents();
	}
	void WindowsWindow::ThatSerpinskiRecursiveFunction(Point A, Point B, Point C, int depth)
	{
		if (depth < 0)
			return;
		depth--;

  		float timeValue = glfwGetTime();
  		m_Shader->SetUniformValue("time", timeValue * depth);

		float xx = (A.x + B.x) / 2;
		float xy = (A.y + B.y) / 2;
		Point X(xx, xy, 0.f);
		float yx = (A.x + C.x) / 2;
		float yy = (A.y + C.y) / 2;
		Point Y(yx, yy, 0.f);
		float zx = (B.x + C.x) / 2;
		float zy = (B.y + C.y) / 2;
		Point Z(zx, zy, 0.f);

		float vertices[16] = {
			X.x,X.y,X.z, 
			Y.x,Y.y,Y.z, 
			Z.x,Z.y,Z.z,
			1.0f,1.0f,
			1.0f,0.0f,
			0.0f,0.0f,
		};

		Buffer buffer(GL_ARRAY_BUFFER);
		buffer.Bind();
		buffer.SetData(vertices, 16, GL_STATIC_DRAW);

		VertexArray vao;
		vao.Bind();
		vao.SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		vao.EnableAttribArray(0);
		vao.SetAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(9 * sizeof(float)));
		vao.EnableAttribArray(1);

// 		unsigned int indices[] = {
// 			0, 1, 2
// 		};
// 		Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
// 		ebo.Bind();
// 		ebo.SetData(indices, 3, GL_STATIC_DRAW);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);


		ThatSerpinskiRecursiveFunction(X, B, Z, depth);
		ThatSerpinskiRecursiveFunction(A, X, Y, depth);
		ThatSerpinskiRecursiveFunction(Y, Z, C, depth);
	}


	void WindowsWindow::DoOpenGlStuff()
	{

		Texture boxTexture("Assets/Textures/WoodenTexture.jpg", GL_RGB);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_Texture = boxTexture.GetHandle();

		m_Shader = std::make_unique<Shader>("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");

		m_Shader->SetUniformValue("v_Texture", 0);

		m_Shader->Bind();
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
