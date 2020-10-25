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
		EventManager::Init(false);
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
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();

			if (m_Window->IsRunning())
				m_Window->OnUpdate();

			m_ImGuiLayer->End();
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
		EventManager::GetInstance().AddListener<WindowCloseEvent>(BIND_FUNC_EVT(Application::OnWindowClose));
		EventManager::GetInstance().AddListener<KeyPressedEvent>(BIND_FUNC_EVT(Application::OnKeyPressed));
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

		//PropagateEvent(e);
	}
}
