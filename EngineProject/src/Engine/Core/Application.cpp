#include "pch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
#include <Engine\EventsManager\Events\EventManager.h>
#include "glad\glad.h"

namespace Engine {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ENGINE_ASSERT(!s_Instance, "Creating a second application!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{

	}

	void Application::Init()
	{
		//m_Window->SetEventCallback(BIND_EVENT_SIMPLE(Application::OnEvent));
		EventManager::GetInstance().AddListener<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
		EventManager::GetInstance().AddListener<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		EventManager::GetInstance().AddListener<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
		EventManager::GetInstance().AddListener<KeyReleasedEvent>(BIND_EVENT_FN(Application::OnKeyReleased));
		EventManager::GetInstance().AddListener<KeyRepeatEvent>(BIND_EVENT_FN(Application::OnKeyRepeat));
		EventManager::GetInstance().AddListener<MouseButtonPressedEvent>(BIND_EVENT_FN(Application::OnMouseButtonPressed));
		EventManager::GetInstance().AddListener<MouseButtonReleasedEvent>(BIND_EVENT_FN(Application::OnMouseButtonReleased));
		EventManager::GetInstance().AddListener<MouseScrollEvent>(BIND_EVENT_FN(Application::OnMouseScrolled));
		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMoved));
		EventManager::GetInstance().AddListener<KeyTypedEvent>(BIND_EVENT_FN(Application::OnKeyTyped));
	}

	void Application::Run()
	{
		Log::Init();
		EventManager::Init();
		m_Window->Init();
		this->Init();


		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
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
	//void Application::OnEvent(EventBase& evt)
	//{
	//	CORE_INFO("{0}", evt);

	//}
	void Application::OnWindowResize(WindowResizeEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}

	void Application::OnWindowClose(WindowCloseEvent& e)
	{
		CORE_INFO("{0}", e);
		m_Running = false;
	}
	void Application::OnKeyPressed(KeyPressedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnKeyRepeat(KeyRepeatEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseScrolled(MouseScrollEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnMouseMoved(MouseMovedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
	void Application::OnKeyTyped(KeyTypedEvent& e)
	{
		CORE_INFO("{0}", e);
		PropagateEvent(e);
	}
}
