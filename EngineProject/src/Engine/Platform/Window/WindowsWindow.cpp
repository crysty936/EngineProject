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
#include <glm/gtc/matrix_transform.hpp>
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
		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Texture2);

		m_vertexArray1->Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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

// 		constexpr float vertices[] = {
// 		 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,					//triangle and colors
// 		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
// 		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
// 		};


		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
		};
		Buffer buffer(GL_ARRAY_BUFFER);
		buffer.Bind();
		buffer.SetData(vertices, 32, GL_STATIC_DRAW);

		VertexArray* VAO = new VertexArray();
		VAO->Bind();
		buffer.Bind();
		VAO->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		VAO->EnableAttribArray(0);
		VAO->SetAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		VAO->EnableAttribArray(1);
		VAO->SetAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		VAO->EnableAttribArray(2);
		m_vertexArray1 = VAO;


		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		Buffer ebo(GL_ELEMENT_ARRAY_BUFFER);
		ebo.Bind();
		ebo.SetData(indices, 6, GL_STATIC_DRAW);

		Texture boxTexture("Assets/Textures/WoodenTexture.jpg", GL_RGB);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Texture smileyTexture("Assets/Textures/AwesomefaceTexture.png", GL_RGBA);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_Texture = boxTexture.GetHandle();
		m_Texture2 = smileyTexture.GetHandle();

		m_Shader = std::make_unique<Shader>("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");

		m_Shader->Bind();

		m_Shader->SetUniformValue("v_Texture1", 0);
		m_Shader->SetUniformValue("v_Texture2", 1);



		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));















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
