#include "pch.h"
#include "Camera.h"
#include "Engine/Platform/RenderingApi/OpenGL/OpenGLUtils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

	Camera::Camera(float InWindowWidth, float InWindowHeight, glm::vec3 InCameraPos, glm::vec3 InCameraFront)
		:CameraPos(InCameraPos), CameraFront(InCameraFront), MouseLastX(InWindowWidth / 2), MouseLastY(InWindowHeight / 2)
	{
		EventManager::GetInstance().AddListener<MouseMovedEvent>(BIND_FUNC_EVT(Camera::OnMouseMoved));
	}

	Camera::~Camera()
	{
		EventManager::GetInstance().RemoveListener<MouseMovedEvent>(BIND_FUNC_EVT(Camera::OnMouseMoved));
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
			break;
		}
		case CameraDirection::Up:
		{
			glm::vec3 Right = glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up));
			glm::vec3 Up = -glm::cross(CameraFront, Right);
			CameraPos += Up * Amount;
			break;
		}
		case CameraDirection::Down:
		{
			glm::vec3 Right = glm::normalize(glm::cross(CameraFront, GLMStatics::Vec3Up));
			glm::vec3 Down = glm::cross(CameraFront, Right);
			CameraPos += Down * Amount;
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

		const float sensitivity = 0.1f;
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
		//const glm::mat4 LookAt = glm::lookAt(CameraPos, CameraPos + CameraFront, GLMStatics::Vec3Up);
		//return LookAt;

		glm::vec3 ThisFront = glm::normalize(-CameraFront);
		glm::vec3 Right = glm::normalize(glm::cross(GLMStatics::Vec3Up, ThisFront));
		glm::vec3 CameraUp = glm::cross(ThisFront, Right);

		float firstMatrixA[16] =
		{
			Right.x,		Right.y,		Right.z,		0,
			CameraUp.x,		CameraUp.y,		CameraUp.z,		0,
			ThisFront.x,	ThisFront.y,	ThisFront.z,	0,
			0,				0,				0,				1
		};

		//glm::mat4 FirstMatrix = glm::make_mat4(firstMatrixA);

		glm::mat4 FirstMatrix = glm::mat4(1.f);
		memcpy(glm::value_ptr(FirstMatrix), firstMatrixA, sizeof(glm::mat4));

		FirstMatrix = glm::transpose(FirstMatrix);

		float SecondMatrixA[16] =
		{
			1,		0,		0,		-CameraPos.x,
			0,		1,		0,		-CameraPos.y,
			0,		0,		1,		-CameraPos.z,
			0,		0,		0,		1
		};

		//glm::mat4 SecondMatrix = glm::make_mat4(SecondMatrixA);

		glm::mat4 SecondMatrix = glm::mat4(1.f);
		memcpy(glm::value_ptr(SecondMatrix), SecondMatrixA, sizeof(glm::mat4));
		SecondMatrix = glm::transpose(SecondMatrix);


		//  		glm::mat4 FirstMatrix2 = glm::mat4(1.0f);
		//  		FirstMatrix2[0][0] = Right.x;
		//  		FirstMatrix2[1][0] = Right.y;
		//  		FirstMatrix2[2][0] = Right.z;
		//  		FirstMatrix2[0][1] = CameraUp.x;
		//  		FirstMatrix2[1][1] = CameraUp.y;
		//  		FirstMatrix2[2][1] = CameraUp.z;
		//  		FirstMatrix2[0][2] = ThisFront.x;
		//  		FirstMatrix2[1][2] = ThisFront.y;
		//  		FirstMatrix2[2][2] = ThisFront.z;
		//  
		//  
		//  		glm::mat4 SecondMatrix2 = glm::mat4(1.0f);
		//  		SecondMatrix2[3][0] = -CameraPos.x;
		//  		SecondMatrix2[3][1] = -CameraPos.y;
		//  		SecondMatrix2[3][2] = -CameraPos.z;

		//glm::mat4 myLookAt = FirstMatrix2 * SecondMatrix2;

		glm::mat4 myLookAt = FirstMatrix * SecondMatrix;

		return myLookAt;

	}
}
