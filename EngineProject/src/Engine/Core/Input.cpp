#include "pch.h"
#include "Engine/Core/Input.h"
#include "GLFW/glfw3.h"


bool Engine::Input::IsKeyPressed(KeyCode key)
{
	return false;
}

bool Engine::Input::IsMouseButtonPressed(MouseCode button)
{
	return false;
}

Vector2D Engine::Input::GetMousePosition()
{
	return Vector2D();
}

float Engine::Input::GetMouseX()
{
	return 0.0f;
}

float Engine::Input::GetMouseY()
{
	return 0.0f;
}


