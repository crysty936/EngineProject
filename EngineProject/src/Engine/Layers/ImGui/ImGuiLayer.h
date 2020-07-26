#pragma once
#include "Engine/Core/Layer.h"
#include "Engine/EventsManager/Events/EventManager.h"

struct ImGuiIO;

namespace Engine
{
	class ENGINE_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUpdate() override;
		void OnAttach() override;
		void OnDetach() override;

	private:
		float m_Time = 0.0f;
		ImGuiIO* m_Io = nullptr;

	private:
		void SetKeys();
		void OnWindowResize(WindowResizeEvent& e);
		void OnMouseMoved(MouseMovedEvent& e);
		void OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		void OnMouseScroll(MouseScrollEvent& e);
		void OnKeyPressed(KeyPressedEvent& e);
		void OnKeyReleased(KeyReleasedEvent& e);
		void OnKeyTyped(KeyTypedEvent& e);


	};






}
