#include "pch.h"
#include "WindowsWindow.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include <glad/glad.h>
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include "Engine/Renderer/VertexBuffer.h"

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
		m_RenderingContext->SwapBuffers();
		glfwPollEvents();




		m_Shader->Bind();
		glBindVertexArray(m_vertexArray1);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(m_vertexArray2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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


		constexpr float triangle1Vertices[] =
		{
			 0.0f, -0.5f, 0.0f,
			-1.0f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		constexpr float triangle2Vertices[] =
		{
			1.0f,-0.5f,0.0f,
			0.0f, -0.5f, 0.0f,
			0.5f,0.5f,0.0f,
		};
		constexpr float colours[] = {
			1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};

		VertexBuffer buffer1;
		buffer1.Bind();
		buffer1.SetData(triangle1Vertices, sizeof(triangle1Vertices), GL_STATIC_DRAW);

		VertexBuffer buffer2;
		buffer2.Bind();
		buffer2.SetData(triangle2Vertices, sizeof(triangle2Vertices), GL_STATIC_DRAW);

		VertexBuffer colorBuffer;
		colorBuffer.Bind();
		colorBuffer.SetData(colours, sizeof(colours), GL_STATIC_DRAW);



		unsigned int vertexArray1;
		glGenVertexArrays(1, &vertexArray1);
		glBindVertexArray(vertexArray1);

		buffer1.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		colorBuffer.Bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		unsigned int vertexArray2;
		glGenVertexArrays(1, &vertexArray2);
		glBindVertexArray(vertexArray2);

		buffer2.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		colorBuffer.Bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);



		// 
		// 
		// 
		// 
		// 
		// 		unsigned int indexBuffer;
		// 		glGenBuffers(1, &indexBuffer);
		// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		// 
		// 		unsigned int indices[] = {
		// 			0, 1, 3,
		// 			1, 2, 3
		// 		};

				//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				//m_indexBuffer = indexBuffer;


		m_vertexArray1 = vertexArray1;
		m_vertexArray2 = vertexArray2;


		//shader Stuff

		std::string vertexShaderSrc = R"(
			#version 330 core 

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;

			out vec3 v_Position;
			out vec3 v_Color;

			void main()
			{
				v_Position=a_Position;
				v_Color=a_Color;
				gl_Position= vec4(a_Position , 1.0);
	
			}
		)";


		std::string fragmentShaderSrc = R"(
			#version 330 core 

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;
			in vec3 v_Color;

			void main()
			{
				color = vec4(v_Color,1.0);
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexShaderSrc, fragmentShaderSrc);

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
