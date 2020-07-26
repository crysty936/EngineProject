#include "pch.h"
#include "ImGuiLayer.h"
#include "Engine/OpenGL/ImGuiRenderer.h"
#include "Engine/Core/Application.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Input.h"

namespace Engine
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnUpdate()
	{

		static bool show = true;

		float time = (float)glfwGetTime();

		m_Io->DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		m_Io = &ImGui::GetIO();

		m_Io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		m_Io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		SetKeys();
		ImGui_ImplOpenGL3_Init("#version 410");

		Application& app = Application::GetInstance();
		m_Io->DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		EventManager& instance = EventManager::GetInstance();
		instance.AddListener<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResize), this);
		instance.AddListener<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMoved), this);
		instance.AddListener<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed), this);
		instance.AddListener<MouseScrollEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScroll), this);
		instance.AddListener<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressed), this);
		instance.AddListener<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleased), this);
		instance.AddListener<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased), this);
		instance.AddListener<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTyped), this);
	}

	void ImGuiLayer::OnDetach()
	{
		EventManager& instance = EventManager::GetInstance();
		instance.RemoveListener<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResize), this);
		instance.RemoveListener<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMoved), this);
		instance.RemoveListener<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed), this);
		instance.RemoveListener<MouseScrollEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScroll), this);
		instance.RemoveListener<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressed), this);
		instance.RemoveListener<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleased), this);
		instance.RemoveListener<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleased), this);
		instance.RemoveListener<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTyped), this);
	}

	void ImGuiLayer::SetKeys()
	{
		m_Io->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		m_Io->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		m_Io->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		m_Io->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		m_Io->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		m_Io->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		m_Io->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		m_Io->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		m_Io->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		m_Io->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		m_Io->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		m_Io->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		m_Io->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		m_Io->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		m_Io->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		m_Io->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		m_Io->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		m_Io->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		m_Io->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		m_Io->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		m_Io->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
	}

	void ImGuiLayer::OnWindowResize(WindowResizeEvent& e)
	{
		m_Io->DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		e.SetIsHandled(true);
	}

	void ImGuiLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		m_Io->MousePos = ImVec2(e.MouseX, e.MouseY);
		e.SetIsHandled(true);
	}

	void ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		m_Io->MouseDown[e.Button] = true;
		e.SetIsHandled(true);
	}

	void ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		m_Io->MouseDown[e.Button] = false;
		e.SetIsHandled(true);
	}

	void ImGuiLayer::OnMouseScroll(MouseScrollEvent& e)
	{
		m_Io->MouseWheelH += e.XOffset;
		m_Io->MouseWheel += e.YOffset;
		e.SetIsHandled(true);
	}
	void ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		m_Io->KeysDown[e.GetKeyCode()] = true;

		m_Io->KeyCtrl = m_Io->KeysDown[GLFW_KEY_LEFT_CONTROL] || m_Io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
		m_Io->KeyShift = m_Io->KeysDown[GLFW_KEY_LEFT_SHIFT] || m_Io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
		m_Io->KeyAlt = m_Io->KeysDown[GLFW_KEY_LEFT_ALT] || m_Io->KeysDown[GLFW_KEY_RIGHT_ALT];
		m_Io->KeySuper = m_Io->KeysDown[GLFW_KEY_LEFT_SUPER] || m_Io->KeysDown[GLFW_KEY_RIGHT_SUPER];

	}
	void ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
	{
		m_Io->KeysDown[e.GetKeyCode()] = false;

		m_Io->KeyCtrl = m_Io->KeysDown[GLFW_KEY_LEFT_CONTROL] || m_Io->KeysDown[GLFW_KEY_RIGHT_CONTROL];
		m_Io->KeyShift = m_Io->KeysDown[GLFW_KEY_LEFT_SHIFT] || m_Io->KeysDown[GLFW_KEY_RIGHT_SHIFT];
		m_Io->KeyAlt = m_Io->KeysDown[GLFW_KEY_LEFT_ALT] || m_Io->KeysDown[GLFW_KEY_RIGHT_ALT];
		m_Io->KeySuper = m_Io->KeysDown[GLFW_KEY_LEFT_SUPER] || m_Io->KeysDown[GLFW_KEY_RIGHT_SUPER];
	}

	void ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
	{
		if (e.GetKeyCode() > 0 && e.GetKeyCode() < 0x10000)
			m_Io->AddInputCharacter((unsigned short)e.GetKeyCode());
	}


}
