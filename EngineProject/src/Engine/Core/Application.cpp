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
		Log::Init();
		EventManager::Init();
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();

			m_ImGuiLayer->End();

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

	void Application::InitListeners()
	{
		//m_Window->SetEventCallback(BIND_EVENT_SIMPLE(Application::OnEvent));
		EventManager::GetInstance().AddListener<WindowResizeEvent>(BIND_FUNC_OBJ(Application::OnWindowResize));
		EventManager::GetInstance().AddListener<WindowCloseEvent>(BIND_FUNC_OBJ(Application::OnWindowClose));
		EventManager::GetInstance().AddListener<KeyPressedEvent>(BIND_FUNC_OBJ(Application::OnKeyPressed));
		EventManager::GetInstance().AddListener<KeyReleasedEvent>(BIND_FUNC_OBJ(Application::OnKeyReleased));
		EventManager::GetInstance().AddListener<KeyRepeatEvent>(BIND_FUNC_OBJ(Application::OnKeyRepeat));
		EventManager::GetInstance().AddListener<MouseButtonPressedEvent>(BIND_FUNC_OBJ(Application::OnMouseButtonPressed));
		EventManager::GetInstance().AddListener<MouseButtonReleasedEvent>(BIND_FUNC_OBJ(Application::OnMouseButtonReleased));
		EventManager::GetInstance().AddListener<MouseScrollEvent>(BIND_FUNC_OBJ(Application::OnMouseScrolled));
		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_OBJ(Application::OnMouseMoved));
		EventManager::GetInstance().AddListener<KeyTypedEvent>(BIND_FUNC_OBJ(Application::OnKeyTyped));
	}

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
