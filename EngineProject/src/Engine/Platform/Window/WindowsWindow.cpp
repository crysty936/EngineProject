#include "pch.h"
#include "WindowsWindow.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/OpenGLUtils.h"
#include <imgui.h>

static constexpr glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

namespace Engine {

	static bool s_GLFWInitialized = false;



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

		delete m_vertexArray1;
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
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, nullptr);
		DoOpenGlStuff();
	}
	void WindowsWindow::OnUpdate()
	{
		Draw();

		m_RenderingContext->SwapBuffers();
		glfwPollEvents();
	}
	void WindowsWindow::Draw()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Renderer::GetRenderer().Clear();

		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(sides, height, forward));
		m_Shader->SetUniformValue4fv("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		m_Shader->SetUniformValue4fv("projection", projection);

		// 		for (glm::vec3 cubePosition : cubePositions)
		// 		{
		// 			glm::mat4 model = glm::mat4(1.0f);
		// 			model = glm::translate(model, cubePosition);
		// 			m_Shader->SetUniformValue4fv("model", model);
		// 
		// 			glDrawArrays(GL_TRIANGLES, 0, 36);
		// 		}

		glm::mat4 model = glm::mat4(1.0f);
		static glm::vec3 translation(0, 0, 0);
		ImGui::SliderFloat3("Model", &translation.x, 0.0f, 4.0f);
		model = glm::translate(model, translation);
		m_Shader->SetUniformValue4fv("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		if (ImGui::CollapsingHeader("MyHelp"))
		{

			ImGui::Text("ABOUT THIS DEMO:");
			ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
			ImGui::Separator();

			ImGui::Text("PROGRAMMER GUIDE:");
			ImGui::BulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
		}

	}


	void WindowsWindow::DoOpenGlStuff()
	{

		Texture boxTexture("Assets/Textures/WoodenTexture.jpg", GL_RGB);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		m_Texture = boxTexture.GetHandle();

		//m_Shader = std::make_unique<Shader>("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");
		m_Shader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");

		m_Shader->Bind();

		m_Shader->SetUniformValue1i("v_Texture", 0);


		constexpr float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		VertexBuffer buffer;
		buffer.Bind();
		buffer.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

		m_vertexArray1 = new VertexArray();
		VertexBufferLayout Layout;
		Layout.Push<float>(3);
		Layout.Push<float>(2);
		m_vertexArray1->AddBuffer(buffer, Layout);


		// 
		// 		m_vertexArray1->SetAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		// 		m_vertexArray1->EnableAttribArray(0);
		// 		m_vertexArray1->SetAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		// 		m_vertexArray1->EnableAttribArray(1);

				// 		float vertices[] = {
				// 			// positions          // texture coords
				// 			 300.0f,  300.0f, 0.0f,   1.0f, 1.0f, // top right
				// 			 300.0f,  100.0f, 0.0f,   1.0f, 0.0f, // bottom right
				// 			 100.0f,  100.0f, 0.0f,   0.0f, 0.0f, // bottom left
				// 			 100.0f,  300.0f, 0.0f,   0.0f, 1.0f  // top left 
				// 		};
				// 
				// 		unsigned int indices[] = {
				// 			0, 1, 3, // first triangle
				// 			1, 2, 3  // second triangle
				// 		};
				// 		unsigned int VBO, VAO, EBO;
				// 		glGenVertexArrays(1, &VAO);
				// 		glGenBuffers(1, &VBO);
				// 		glGenBuffers(1, &EBO);
				// 
				// 		glBindVertexArray(VAO);
				// 
				// 		glBindBuffer(GL_ARRAY_BUFFER, VBO);
				// 		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				// 
				// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				// 		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
				// 
				// 		// position attribute
				// 		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
				// 		glEnableVertexAttribArray(0);
				// 		// texture coord attribute
				// 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				// 		glEnableVertexAttribArray(1);

		EventManager::GetInstance().AddListener<KeyRepeatEvent>(BIND_FUNC_EVT(WindowsWindow::OnKeyRepeat));
	}

	void WindowsWindow::OnKeyRepeat(KeyRepeatEvent e)
	{

		switch (e.GetKeyCode())
		{
		case (int)KeyCode::W:
		{
			forward += 0.1f;
			break;
		}
		case (int)KeyCode::S:
		{
			forward -= 0.1f;
			break;
		}
		case (int)KeyCode::D:
		{
			sides -= 0.1f;
			break;
		}
		case (int)KeyCode::A:
		{
			sides += 0.1f;
			break;
		}
		case (int)KeyCode::Space:
		{
			height -= 0.1f;
			break;
		}
		case (int)KeyCode::LeftControl:
		{
			height += 0.1f;
			break;
		}
		break;
		}
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
