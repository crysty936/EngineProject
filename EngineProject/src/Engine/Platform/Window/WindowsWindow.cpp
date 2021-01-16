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
#include "Engine/Renderer/Camera.h"

struct asd
{
	int a;
	struct bsd
	{
		int b;


	};

};

static const glm::vec3 LightPosition = glm::vec3(10.2f, 0.0f, 0.0f);

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

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


float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
		float ElapsedTime = glfwGetTime();
		deltaTime = ElapsedTime - lastFrame;
		lastFrame = ElapsedTime;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Renderer::GetRenderer().Clear();

		LightRO->GetShader().Bind();
		LightRO->GetVAO().Bind();

		//camera stuff  
		glm::mat4 View = MainCamera->GetCameraLookAt();
		LightRO->GetShader().SetUniformValue4fv("view", View);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
		LightRO->GetShader().SetUniformValue4fv("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3& translation = LightRO->GetTransform();

		model = glm::translate(model, translation);
		model = glm::scale(model, glm::vec3(0.2f));
		LightRO->GetShader().SetUniformValue4fv("model", model);
		glm::vec3 LightColor;

		LightColor = glm::vec3(1, 1, 1);


		LightRO->GetShader().SetUniformValue3fv("ULightColor", LightColor);
		//

		glDrawArrays(GL_TRIANGLES, 0, 36);

		AddImGuiSlider("Light Object", &translation.x);

// 		const glm::vec4 Vec4ViewSpacePosition = View * glm::vec4(LightRO->GetTransform(), 1.0f);
// 		const glm::vec3 Vec3ViewSpacePosition = glm::vec3(Vec4ViewSpacePosition);

		for (int i = 0; i < RenderObjects.size(); i++)
		{
			const RenderObject& RO = RenderObjects[i];
			Shader& const CurShader = RO.GetShader();
			CurShader.Bind();
			RO.GetVAO().Bind();


			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_TextureSpecular);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_TextureEmission);

			const glm::mat4 View = MainCamera->GetCameraLookAt();
			CurShader.SetUniformValue4fv("view", View);

			CurShader.SetUniformValue3f("UObjectColor", 1.0f, 1.0f, 1.0f);

			const glm::vec3 DiffuseColor = LightColor * glm::vec3(0.5f);
			const glm::vec3 AmbientColor = DiffuseColor * glm::vec3(0.2f);


			CurShader.SetUniformValue3fv("ULight.Position", MainCamera->GetCameraPos());
			CurShader.SetUniformValue3fv("ULight.Direction", MainCamera->GetCameraFront());

			CurShader.SetUniformValue1f("ULight.Constant", 1.0f);
			CurShader.SetUniformValue1f("ULight.Linear", 0.09f);
			CurShader.SetUniformValue1f("ULight.Quadratic", 0.032f);
			CurShader.SetUniformValue1f("ULight.InnerCutOff", glm::cos(glm::radians(12.5f)));
			CurShader.SetUniformValue1f("ULight.OuterCutOff", glm::cos(glm::radians(17.5f)));


			CurShader.SetUniformValue3fv("ULight.Ambient", AmbientColor);
			CurShader.SetUniformValue3fv("ULight.Diffuse", DiffuseColor);
			CurShader.SetUniformValue3f("ULight.Specular", LightColor.x, LightColor.y, LightColor.z);

			CurShader.SetUniformValue1i("UMaterial.DiffuseMap", 0);
			CurShader.SetUniformValue1i("UMaterial.SpecularMap", 1);
			//CurShader.SetUniformValue1i("UMaterial.EmissionMap", 2);
			CurShader.SetUniformValue1f("UMaterial.Shininess", 32.f);


			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
			CurShader.SetUniformValue4fv("projection", projection);

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3& translation = RenderObjects[i].GetTransform();
			model = glm::translate(model, translation);
			CurShader.SetUniformValue4fv("model", model);
			//

			glDrawArrays(GL_TRIANGLES, 0, 36);

			AddImGuiSlider("Model" + std::to_string(i + 1), &translation.x);
		}
	}

	void WindowsWindow::AddImGuiSlider(const std::string& Name, float* Pointer)
	{
		ImGui::SliderFloat3(Name.c_str(), Pointer, -20.0f, 20.0f);
	}

	void WindowsWindow::ProcessInput()
	{
		const float Velocity = MainCamera->GetCameraSpeed() * deltaTime;
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Forward, Velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Backward, Velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Left, Velocity);

		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Right, Velocity);

		}
		if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Up, Velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_C) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Down, Velocity);
		}
	}

	void WindowsWindow::DoOpenGlStuff()
	{
		m_Texture = Texture("Assets/Textures/WoodContainer.png", GL_RGBA).GetHandle();
		m_TextureSpecular = Texture("Assets/Textures/WoodContainer_Specular.png", GL_RGBA).GetHandle();
		m_TextureEmission = Texture("Assets/Textures/WoodContainer_Emission.jpg", GL_RGB).GetHandle();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Shader* ObjectShader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/fragmentShader.glsl");
		Shader* LightObjShader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/LightFragmentShader.glsl");

		VertexBuffer buffer;
		buffer.Bind();
		buffer.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

		VertexArray* ContainerVAO = new VertexArray();
		VertexBufferLayout ContainerLayout;
		ContainerLayout.Push<float>(3);
		ContainerLayout.Push<float>(3);
		ContainerLayout.Push<float>(2);
		ContainerVAO->AddBuffer(buffer, ContainerLayout);

		VertexArray* LightVAO = new VertexArray();
		LightVAO->AddBuffer(buffer, ContainerLayout);

		int arraySize = sizeof(cubePositions) / sizeof(glm::vec3);
		RenderObjects.resize(arraySize);

		for (int i = 0; i < arraySize; i++)
		{
			RenderObject ContainerRO = RenderObject(cubePositions[i], ContainerVAO, ObjectShader);
			RenderObjects[i] = ContainerRO;
		}


		LightRO = new RenderObject(LightPosition, LightVAO, LightObjShader);

		//RenderObjects.push_back(ContainerRO);

		MainCamera = new Camera(GetWidth(), GetHeight(), glm::vec3(0, 0.f, 3.0f), glm::vec3(0, 0.f, -1.0f));
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
