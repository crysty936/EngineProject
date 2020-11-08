#pragma once
#include "glm\glm.hpp"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"

namespace Engine
{
	class Camera
	{
	public:
		Camera(float WindowWidth, float WindowHeight, glm::vec3 CameraPos, glm::vec3 _CameraFront);



	private:

		void OnMouseMoved(MouseMovedEvent e);

		float MouseLastX = 0.f;
		float MouseLastY = 0.f;

		float Yaw = -90.0f;
		float Pitch = 0.0f;

		bool FirstMouse = true;

		glm::vec3 CameraPos = glm::vec3(0, 0, 0);
		glm::vec3 CameraFront = glm::vec3(0, 0, 0);;

	public:

		FORCEINLINE glm::vec3 GetCameraFront() const { return CameraFront; }
		FORCEINLINE glm::vec3 GetCameraPos() const { return CameraPos; }

	};
}