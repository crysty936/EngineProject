#include "pch.h"
#include "Application.h"
#include <Engine\EventsManager\Events\EventManager.h>
#include <GLFW/glfw3.h>
#include <glad\glad.h>

namespace Engine {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ENGINE_CORE_ASSERT(!s_Instance, "Creating a second application!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{

	}

	void Application::Init()
	{
		Log::Init();
		EventManager::Init();
		EventManager::GetInstance().SetTraceEnabled(false);
		m_Window->Init();
		InitListeners();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::Run()
	{
		Init();

		while (m_Running)
		{
// 			glClearColor(1, 0, 1, 1);
// 			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();

			m_ImGuiLayer->End();


			if (m_Window->IsRunning())
			{
				m_Window->OnUpdate(); 
			}
		}
	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}
	void Application::InitListeners()
	{
		EventManager::GetInstance().AddListener<WindowResizeEvent>(BIND_FUNC_EVT(Application::OnWindowResize));
		EventManager::GetInstance().AddListener<WindowCloseEvent>(BIND_FUNC_EVT(Application::OnWindowClose));
		EventManager::GetInstance().AddListener<KeyPressedEvent>(BIND_FUNC_EVT(Application::OnKeyPressed));
		EventManager::GetInstance().AddListener<KeyReleasedEvent>(BIND_FUNC_EVT(Application::OnKeyReleased));
		EventManager::GetInstance().AddListener<KeyRepeatEvent>(BIND_FUNC_EVT(Application::OnKeyRepeat));
		EventManager::GetInstance().AddListener<MouseButtonPressedEvent>(BIND_FUNC_EVT(Application::OnMouseButtonPressed));
		EventManager::GetInstance().AddListener<MouseButtonReleasedEvent>(BIND_FUNC_EVT(Application::OnMouseButtonReleased));
		EventManager::GetInstance().AddListener<MouseScrollEvent>(BIND_FUNC_EVT(Application::OnMouseScrolled));
		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_EVT(Application::OnMouseMoved));
		EventManager::GetInstance().AddListener<KeyTypedEvent>(BIND_FUNC_EVT(Application::OnKeyTyped));
	}

	void Application::OnWindowResize(WindowResizeEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}

	void Application::OnWindowClose(WindowCloseEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		m_Running = false;
	}
	void Application::OnKeyPressed(KeyPressedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		if (e.GetKeyCode() == static_cast<int>(Engine::Key::Escape))
		{
			m_Running = false;
		}

		PropagateEvent(e);
	}
	void Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnKeyRepeat(KeyRepeatEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseScrolled(MouseScrollEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseMoved(MouseMovedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnKeyTyped(KeyTypedEvent& e)
	{
		//LOG_CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
}
