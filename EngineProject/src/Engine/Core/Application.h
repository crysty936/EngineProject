#pragma once
#include "Core.h"
#include "Engine\Platform\Window\Window.h"
#include "Engine\EventsManager\Events\ApplicationEvents.h"
#include "Engine\EventsManager\Events\KeyEvents.h"
#include "Engine\EventsManager\Events\MouseEvents.h"
#include "Engine\Core\LayerStack.h"
#include "Engine\Layers\ImGui\ImGuiLayer.h"

namespace Engine {

	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Init();
		void Run();
		//void OnEvent(EventBase& evt);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& GetInstance() { return *s_Instance; }

	private:
		void InitListeners();

		void OnWindowResize(WindowResizeEvent& e);
		void OnWindowClose(WindowCloseEvent& e);
		void OnKeyPressed(KeyPressedEvent& e);
		void OnKeyReleased(KeyReleasedEvent& e);
		void OnKeyRepeat(KeyRepeatEvent& e);
		void OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		void OnMouseScrolled(MouseScrollEvent& e);
		void OnMouseMoved(MouseMovedEvent& e);
		void OnKeyTyped(KeyTypedEvent& e);



	private:
// 		template<typename T>
// 		void PropagateEvent(T& e)
// 		{
// 			EventManager& instance = EventManager::GetInstance();
// 			for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
// 			{
// 				--it;
// 				if (e.IsHandled())
// 					break;
// 				Layer* layer = (*it);
// 				instance.SendEvent(e, layer);
// 			}
// 		}

	private:

		bool m_Running = true;
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};


	Application* CreateApplication();
}


