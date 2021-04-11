#pragma once
#include "glm\glm.hpp"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include "Engine/EventsManager/Events/EventManager.h"

namespace Engine
{
	enum class CameraDirection : unsigned char
	{
		Forward,
		Backward,
		Left,
		Right,
		Up,
		Down
	};

	class Camera
	{
	public:
		Camera(uint32_t WindowWidth, uint32_t WindowHeight, glm::vec3 CameraPos, glm::vec3 _CameraFront);
		~Camera();

	public:
		void Move(CameraDirection Direction, float Amount);

	public:
		const glm::mat4& GetCameraLookAt() const;
		FORCEINLINE const glm::vec3& GetCameraFront() const { return CameraFront; }
		FORCEINLINE const glm::vec3& GetCameraPos() const { return CameraPos; }
		FORCEINLINE const float GetCameraSpeed() const { return CameraSpeed; }

	private:
		void OnMouseMoved(MouseMovedEvent e);

	private:
		float MouseLastX = 0.f;
		float MouseLastY = 0.f;
		float Yaw = -90.0f;
		float Pitch = 0.0f;
		bool FirstMouse = true;
		glm::vec3 CameraPos = glm::vec3(0, 0, 0);
		glm::vec3 CameraFront = glm::vec3(0, 0, 0);;
		float CameraSpeed = 2.5f;
	};
}