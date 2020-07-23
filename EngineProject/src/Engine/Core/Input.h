#pragma once
#include <Engine\Core\KeyCodes.h>
#include <Engine\Core\MouseCodes.h>

struct Vector2D
{
	float x;
	float y;
};


namespace Engine
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static Vector2D GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}