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
#include "Engine/Renderer/TextureUtils.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Renderer/Renderer.h"
#include <imgui.h>
#include <sstream>
#include "Engine/Renderer/RenderObject.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/Model.h"
#include "Engine/Parser/GLTFParser.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLContext.h"
#include "Engine/Renderer/Shader.h"

static const glm::vec3 LightPosition = glm::vec3(1.2f, 0.0f, 0.0f);

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
	//glm::vec3(0.0f,  0.0f,  0.0f),
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

static const glm::vec3 PointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

double deltaTime = 0.0f;
double lastFrame = 0.0f;

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

		m_RenderingContext = std::make_unique<OpenGLContext>(m_Window);

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
		double ElapsedTime = glfwGetTime();
		deltaTime = ElapsedTime - lastFrame;
		//LOG_CORE_INFO("Current Frame Time : {0}", deltaTime);
		lastFrame = ElapsedTime;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Renderer::GetRenderer().Clear();
		//
		glm::vec3 LightColor = glm::vec3(1, 1, 1);

		{

			LightRO->GetShader().Bind();
			LightRO->GetVAO().Bind();

			glm::mat4 View = MainCamera->GetCameraLookAt();
			LightRO->GetShader().SetUniformValue4fv("view", View);

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
			LightRO->GetShader().SetUniformValue4fv("projection", projection);

			glm::mat4 model = glm::mat4(1.0f);
			glm::vec3 translation = MainPosition;

			model = glm::translate(model, translation);
			model = glm::scale(model, glm::vec3(0.2f));
			LightRO->GetShader().SetUniformValue4fv("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			AddImGuiSlider("Light Object", &MainPosition.x);
		}


		{
			ModelShader->Bind();
			const glm::mat4 View = MainCamera->GetCameraLookAt();
			ModelShader->SetUniformValue4fv("view", View);

			glm::mat4 ModelProjection;
			ModelProjection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
			ModelShader->SetUniformValue4fv("projection", ModelProjection);

			glm::mat4 ModelModel = glm::mat4(1.0f);
			glm::vec3& ModelTranslation = ModelPosition;
			ModelModel = glm::translate(ModelModel, ModelTranslation);
			ModelShader->SetUniformValue4fv("model", ModelModel);
			AddImGuiSlider("Model", &ModelTranslation.x);


			ModelShader->SetUniformValue1f("UMaterial.Shininess", 32.f);

			const glm::vec3 DiffuseColor = glm::vec3(0.5f);
			const glm::vec3 AmbientColor = DiffuseColor * glm::vec3(0.2f);

			ModelShader->SetUniformValue3fv("USpotLight.Position", MainCamera->GetCameraPos());
			ModelShader->SetUniformValue3fv("USpotLight.Direction", MainCamera->GetCameraFront());
			ModelShader->SetUniformValue1f("USpotLight.Constant", 1.0f);
			ModelShader->SetUniformValue1f("USpotLight.Linear", 0.09f);
			ModelShader->SetUniformValue1f("USpotLight.Quadratic", 0.032f);
			ModelShader->SetUniformValue1f("USpotLight.InnerCutOff", glm::cos(glm::radians(12.5f)));
			ModelShader->SetUniformValue1f("USpotLight.OuterCutOff", glm::cos(glm::radians(17.5f)));
			ModelShader->SetUniformValue3fv("USpotLight.Ambient", AmbientColor);
			ModelShader->SetUniformValue3fv("USpotLight.Diffuse", DiffuseColor);
			ModelShader->SetUniformValue3f("USpotLight.Specular", LightColor.x, LightColor.y, LightColor.z);

			if (TheModel)
				TheModel->Draw(*ModelShader);
		}
// 
// 		{
// 			ParserModelShader->Bind();
// 			const glm::mat4 View = MainCamera->GetCameraLookAt();
// 			ParserModelShader->SetUniformValue4fv("view", View);
// 
// 			glm::mat4 ModelProjection;
// 			ModelProjection = glm::perspective(glm::radians(45.0f), (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);
// 			ParserModelShader->SetUniformValue4fv("projection", ModelProjection);
// 
// 			glm::mat4 ModelModel = glm::mat4(1.0f);
// 			glm::vec3& ModelTranslation = ParserModelPosition;
// 			ModelModel = glm::translate(ModelModel, ModelTranslation);
// 			ParserModelShader->SetUniformValue4fv("model", ModelModel);
// 			AddImGuiSlider("Model Parser", &ModelTranslation.x);
// 
// 			ParserModelShader->SetUniformValue1f("UMaterial.Shininess", 32.f);
// 
// 			const glm::vec3 DiffuseColor = glm::vec3(0.5f);
// 			const glm::vec3 AmbientColor = DiffuseColor * glm::vec3(0.2f);
// 
// 
// 			ParserModelShader->SetUniformValue3fv("USpotLight.Position", MainCamera->GetCameraPos());
// 			ParserModelShader->SetUniformValue3fv("USpotLight.Direction", MainCamera->GetCameraFront());
// 			ParserModelShader->SetUniformValue1f("USpotLight.Constant", 1.0f);
// 			ParserModelShader->SetUniformValue1f("USpotLight.Linear", 0.09f);
// 			ParserModelShader->SetUniformValue1f("USpotLight.Quadratic", 0.032f);
// 			ParserModelShader->SetUniformValue1f("USpotLight.InnerCutOff", glm::cos(glm::radians(12.5f)));
// 			ParserModelShader->SetUniformValue1f("USpotLight.OuterCutOff", glm::cos(glm::radians(17.5f)));
// 			ParserModelShader->SetUniformValue3fv("USpotLight.Ambient", AmbientColor);
// 			ParserModelShader->SetUniformValue3fv("USpotLight.Diffuse", DiffuseColor);
// 			ParserModelShader->SetUniformValue3f("USpotLight.Specular", LightColor.x, LightColor.y, LightColor.z);
// 
// 			ParserModel->Draw(*ParserModelShader);
// 		}


	
	}

	void WindowsWindow::AddImGuiSlider(const std::string& inName, float* inPtr)
	{
		ImGui::SliderFloat3(inName.c_str(), inPtr, -20.0f, 20.0f);
	}

	void WindowsWindow::ProcessInput()
	{
		const float velocity = MainCamera->GetCameraSpeed() * static_cast<float>(deltaTime);
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Forward, velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Backward, velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Left, velocity);

		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Right, velocity);

		}
		if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Up, velocity);
		}
		if (glfwGetKey(m_Window, GLFW_KEY_C) == GLFW_PRESS)
		{
			MainCamera->Move(CameraDirection::Down, velocity);
		}
	}

	void WindowsWindow::DoOpenGlStuff()
	{
		/// <summary>
		TextureUtils::LoadTexture("Assets/Textures/WoodContainer.png", m_Texture);
		TextureUtils::LoadTexture("Assets/Textures/WoodContainer_Specular.png", m_TextureSpecular);
		TextureUtils::LoadTexture("Assets/Textures/WoodContainer_Emission.jpg", m_TextureEmission);

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

		/// /// </summary>

		ModelShader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/BackpackFragmentShader.glsl");
		ParserModelShader = new Shader("Assets/Shaders/vertexShader.glsl", "Assets/Shaders/BackpackFragmentShader.glsl");
		const char* modelPath = "Assets/Models/Backpack/backpack.obj";
		//TheModel = std::make_unique<class Model>(modelPath);

		//ParserModel = GLTFParser::GetInstance().ParseGetModel("Assets/Models/Backpack_glTF/scene.gltf"); //TODO
		TheModel = GLTFParser::GetInstance().ParseGetModel("Assets/Models/TestModelglTF/TestModel.gltf");


		MainCamera = std::make_unique<Camera>(GetWidth(), GetHeight(), glm::vec3(0, 0.f, 3.0f), glm::vec3(0, 0.f, -1.0f));
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
