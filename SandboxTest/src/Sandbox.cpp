#include "Engine.h"
#include "Engine/EventsManager/Events/EventManager.h"
#include "imgui/imgui.h"

class ExampleLayer :public Engine::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnUpdate() override
	{


	}

	void OnImGuiDraw() override
	{

		//ImGui::Begin("Test");
		//ImGui::Text("Hellow World");
		//ImGui::End();


	}


	void OnAttach() override
	{
		Engine::EventManager& instance = Engine::EventManager::GetInstance();
		instance.AddListener<Engine::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::OnKeyPressed), this);
	}

	void OnDetach() override
	{
		Engine::EventManager& instance = Engine::EventManager::GetInstance();
		instance.RemoveListener<Engine::KeyPressedEvent>(BIND_EVENT_FN(ExampleLayer::OnKeyPressed), this);
	}

	void OnKeyPressed(Engine::KeyPressedEvent& e)
	{

	}

};


class Sandbox :public Engine::Application
{
public:

	static Sandbox* GetInstance()
	{
		static Sandbox instance;
		return &instance;
	}
	void Init() override
	{
		Application::Init();
		PushLayer(new ExampleLayer());
	}

private:
	static Sandbox s_Instance;
	Sandbox()
	{
		
	}
	virtual ~Sandbox() override
	{
	}
public:
	Sandbox(Sandbox const& var) = delete;
	void operator=(Sandbox const& var) = delete;
};

Engine::Application* Engine::CreateApplication()
{
	return Sandbox::GetInstance();
}