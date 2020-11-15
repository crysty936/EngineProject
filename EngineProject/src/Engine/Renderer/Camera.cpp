#include "pch.h"
#include "Camera.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

	Camera::Camera(float InWindowWidth, float InWindowHeight, glm::vec3 InCameraPos, glm::vec3 InCameraFront)
		:CameraPos(InCameraPos), CameraFront(InCameraFront), MouseLastX(InWindowWidth / 2), MouseLastY(InWindowHeight / 2)
	{
		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_EVT(Camera::OnMouseMoved));
	}

	void Camera::Move(CameraDirection Direction, float Amount)
	{
		switch (Direction)
		{
		case CameraDirection::Forward:
		{
			CameraPos += CameraFront * Amount;
			break;
		}
		case CameraDirection::Backward:
		{
			CameraPos -= CameraFront * Amount;
			break;
		}
		case CameraDirection::Left:
		{
			glm::vec3 Right = glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up));
			CameraPos -= Right * Amount;
			break;
		}
		case CameraDirection::Right:
		{
			glm::vec3 Right = glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up));
			CameraPos += Right * Amount;

			//glm::vec3 CameraUp = glm::normalize(Right, CameraFront);
			break;
		}
		}



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

	const glm::mat4& Camera::GetCameraLookAt() const
	{
		const glm::mat4 LookAt = glm::lookAt(CameraPos, CameraPos + CameraFront, GLMStatics::Vec3Up);
		return LookAt;
	}

}
