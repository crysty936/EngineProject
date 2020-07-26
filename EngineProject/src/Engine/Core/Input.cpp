#include "pch.h"
#include "Engine/Core/Input.h"
#include "GLFW/glfw3.h"
#include "Engine/Core/Application.h"

namespace Engine
{
	bool Engine::Input::IsKeyPressed(KeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Engine::Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	Vector2D Engine::Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return Vector2D((float)xPos,(float)yPos);
	}

	float Engine::Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Engine::Input::GetMouseY()
	{
		return GetMousePosition().y;
	}
}