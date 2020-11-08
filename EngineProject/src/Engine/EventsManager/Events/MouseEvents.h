#pragma once
#include "Engine/EventsManager/Events/EventBase.h"
#include "Engine/Core/Input.h"



namespace Engine
{
	class MouseMovedEvent : public EventBase
	{
	public:
		MouseMovedEvent(double x, double y)
			:MouseX(x), MouseY(y) {}

		static std::string GetStaticName() { return "MouseMovedEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " x: " << MouseX << ", y: " << MouseY;
			return ss.str();
		}
		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }


		double MouseX, MouseY;
	};

	class MouseButtonPressedEvent :public EventBase
	{
	public:
		MouseButtonPressedEvent(int button)
			:Button(button) {}
		static std::string GetStaticName() { return "MouseButtonPressedEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " " << Button;
			return ss.str();
		}
		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }


		int Button;
	};
	class MouseButtonReleasedEvent :public EventBase
	{
	public:
		MouseButtonReleasedEvent(int button)
			:Button(button) {}
		static std::string GetStaticName() { return "MouseButtonReleased"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " " << Button;
			return ss.str();
		}
		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }


		int Button;
	};
	class MouseScrollEvent :public EventBase
	{
	public:
		MouseScrollEvent(float x, float y)
			:XOffset(x), YOffset(y) {}
		static std::string GetStaticName(){return "MouseScrollEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " X: " << XOffset << ", Y: " << YOffset;
			return ss.str();
		}
		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }

		float XOffset, YOffset;
	};

}



