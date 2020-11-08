#pragma once
#include "glm\glm.hpp"

namespace Engine
{
	class Camera
	{
	public:
		//Camera(float WindowWidth, float WindowHeight, );



	private:

		float MouseLastX = 0.f;
		float MouseLastY = 0.f;

		float Yaw = -90.0f;
		float Pitch = 0.0f;

		bool FirstMouse = true;

		glm::vec3 CameraPos;
		glm::vec3 CameraFront;

	};
}