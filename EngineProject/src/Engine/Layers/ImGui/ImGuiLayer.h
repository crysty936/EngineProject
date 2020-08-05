#pragma once
#include "Engine/Core/Layer.h"
#include "Engine/EventsManager/Events/EventManager.h"


namespace Engine
{
	class ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiDraw() override;

		void Begin();
		void End();


	private:
		float m_Time = 0.0f;

	private:
		void CreateContext();
		void SetStyle();
		void SetRendererBindings();

	};






}
