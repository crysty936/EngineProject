#pragma once
#include <Engine\Core\KeyCodes.h>
#include <Engine\Core\MouseCodes.h>

struct Vector2D
{
	Vector2D(float _x, float _y)
		:x(_x),y(_y){}

	float x;
	float y;
};


namespace Engine
{
	class ENGINE_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static Vector2D GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}