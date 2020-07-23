#pragma once

#include "pch.h"
#include "Engine/Core/Core.h"



namespace Engine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4

	};

#define STRINGIFY(eventType) #eventType


	class EventBase
	{
	public:
		virtual ~EventBase() = default;

		const bool inline IsHandled() const { return m_Handled; }
		void SetIsHandled(bool value) { m_Handled = value; }

		//virtual const char* GetName() const = 0;
		virtual std::string ToString() const = 0;

		//virtual const EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	private:
		bool m_Handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const EventBase& e)
	{
		return os << e.ToString();
	}
}