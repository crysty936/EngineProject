#pragma once
#include "glm\glm.hpp"

namespace Engine
{
	class Camera
	{
	public:
		Camera();



	private:


		float Yaw = -90.0f;
		float Pitch = 0.0f;

		bool FirstMouse = true;

		glm::vec3 CameraPos;
		glm::vec3 CameraFront;

		const glm::vec3 Vec3Up = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}