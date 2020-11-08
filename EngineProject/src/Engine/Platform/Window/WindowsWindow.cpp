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
#include <imgui.h>
#include <sstream>
#include "Engine/Renderer/RenderObject.h"


static glm::vec3 cubePositions[] = {
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

static float vertices[] = {
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

glm::vec3 CameraPos = glm::vec3(0, 0.f, 3.0f);
glm::vec3 CameraFront = glm::vec3(0.f, 0.f, -1.f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float MouseLastX = 640;
float MouseLastY = 360;


float Yaw = -90.f;
float Pitch = 0.f;

bool FirstMouse = true;

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
		ProcessInput();
		Draw();

		m_RenderingContext->SwapBuffers();
		glfwPollEvents();
	}
	void WindowsWindow::Draw()
	{

		//delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Renderer::GetRenderer().Clear();

		m_Shader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);


		//camera stuff
		glm::mat4 View = glm::lookAt(
			CameraPos,
			CameraPos + CameraFront,
			GLMStatics::Vec3Up);

		m_Shader->SetUniformValue4fv("view", View);


		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
		m_Shader->SetUniformValue4fv("projection", projection);


		//render objects
		for (int i = 0; i < RenderObjects.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3* translation = RenderObjects[i].GetTransform();
			model = glm::translate(model, *translation);
			m_Shader->SetUniformValue4fv("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			std::string sliderName("Model");

			sliderName += std::to_string(i + 1);
			ImGui::SliderFloat3(sliderName.c_str(), &translation->x, -4.0f, 4.0f);
		}
	}

	void WindowsWindow::ProcessInput()
	{
		const float CameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			CameraPos += CameraSpeed * CameraFront;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			CameraPos -= CameraSpeed * CameraFront;
		}

		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			CameraPos -= glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up)) * CameraSpeed;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			CameraPos += glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up)) * CameraSpeed;
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

		m_Shader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");

		m_Shader->Bind();

		m_Shader->SetUniformValue1i("v_Texture", 0);

		VertexBuffer buffer;
		buffer.Bind();
		buffer.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

		m_vertexArray1 = new VertexArray();
		VertexBufferLayout Layout;
		Layout.Push<float>(3);
		Layout.Push<float>(2);
		m_vertexArray1->AddBuffer(buffer, Layout);

		int arraySize = sizeof(cubePositions) / sizeof(glm::vec3);
		RenderObjects.resize(arraySize);

		for (int i = 0; i < arraySize; i++)
		{
			RenderObjects[i].SetTransform(cubePositions[i]);
		}


		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_EVT(WindowsWindow::OnMouseMoved));

	}

	void WindowsWindow::OnMouseMoved(MouseMovedEvent e)
	{
		if (FirstMouse)
		{
			MouseLastX = e.MouseX;
			MouseLastY = e.MouseY;
			FirstMouse = false;
		}

		float XOffset = e.MouseX - MouseLastX;
		float YOffset = (-1)*(e.MouseY - MouseLastY);

		MouseLastX = e.MouseX;
		MouseLastY = e.MouseY;

		static const float sensitivity = 0.1f;
		XOffset *= sensitivity;
		YOffset *= sensitivity;

		Yaw += XOffset;
		Pitch += YOffset;


		glm::vec3 direction;
		direction.x = cos(glm::radians(Yaw)*cos(glm::radians(Pitch)));
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw)*cos(glm::radians(Pitch)));
		CameraFront = glm::normalize(direction);
	}


	void WindowsWindow::SetGlfwCallbacks()
	{
		glfwSetWindowSizeCallback(m_Window, 
			[](GLFWwindow* window, int width, int height)
			{
				WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
				data->Width = width;
				data->Height = height;

				WindowResizeEvent e = WindowResizeEvent(width, height);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetFramebufferSizeCallback(m_Window, 
			[](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);

				WindowBufferResizeEvent e = WindowBufferResizeEvent(width, height);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetWindowCloseCallback(m_Window, 
			[](GLFWwindow* window)
			{
				WindowCloseEvent e = WindowCloseEvent(window);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetKeyCallback(m_Window, 
			[](GLFWwindow*, int keycode, int scandCode, int action, int mods)
			{
				EventManager& instance = EventManager::GetInstance();

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e = KeyPressedEvent(keycode);
					instance.SendEvent(e);
					KeyEvent BasicKeyEvent = KeyEvent(keycode);
					instance.SendEvent(BasicKeyEvent);
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
					KeyEvent BasicKeyEvent = KeyEvent(keycode);
					instance.SendEvent(BasicKeyEvent);
					break;
				}
				}
			});

		glfwSetMouseButtonCallback(m_Window, 
			[](GLFWwindow*, int button, int action, int mods)
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
		glfwSetScrollCallback(m_Window, 
			[](GLFWwindow*, double x, double y)
			{
				MouseScrollEvent e = MouseScrollEvent((float)x, (float)y);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetCursorPosCallback(m_Window, 
			[](GLFWwindow*, double x, double y)
			{
				MouseMovedEvent e = MouseMovedEvent((float)x, (float)y);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetCharCallback(m_Window, 
			[](GLFWwindow* window, unsigned int keycode)
			{
				KeyTypedEvent e = KeyTypedEvent(keycode);
				EventManager::GetInstance().SendEvent(e);
			});

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double xPos, double yPos)
			{
				MouseMovedEvent Ev = MouseMovedEvent(xPos, yPos);
				EventManager::GetInstance().SendEvent(Ev);
			});
	}
}
