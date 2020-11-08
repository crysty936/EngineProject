#include "pch.h"
#include "Camera.h"

namespace Engine
{

	Camera::Camera(float WindowWidth, float WindowHeight, glm::vec3 _CameraPos, glm::vec3 _CameraFront)
	{
		CameraPos = _CameraPos;
		CameraFront = _CameraFront;

		MouseLastX = WindowWidth / 2;
		MouseLastY = WindowHeight / 2;

		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_EVT(Camera::OnMouseMoved));
	}

	void Camera::OnMouseMoved(MouseMovedEvent e)
	{
		if (FirstMouse)
		{
			MouseLastX = e.MouseX;
			MouseLastY = e.MouseY;
			FirstMouse = false;
		}

		float XOffset = e.MouseX - MouseLastX;
		float YOffset = (-1) * (e.MouseY - MouseLastY);

		MouseLastX = e.MouseX;
		MouseLastY = e.MouseY;

		static const float sensitivity = 0.1f;
		XOffset *= sensitivity;
		YOffset *= sensitivity;

		Yaw += XOffset;
		Pitch += YOffset;


		glm::vec3 direction;
		direction.x = cos(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		direction.y = sin(glm::radians(Pitch));
		direction.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
		CameraFront = glm::normalize(direction);


	}

}
