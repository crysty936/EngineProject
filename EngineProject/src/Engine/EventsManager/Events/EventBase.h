#pragma once

#include "pch.h"
#include "Engine/Core/Core.h"



namespace Engine
{
	enum class EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4

	};
	inline bool operator&(const int& value, const EventCategory& category)
	{
		return static_cast<int>(static_cast<int>(category)& static_cast<int>(value));
	}
	inline int operator|(const EventCategory& ctg1, const EventCategory& ctg2)
	{
		return static_cast<int>(static_cast<int>(ctg1)|static_cast<int>(ctg2));
	}

#define STRINGIFY(eventType) #eventType


	class EventBase
	{
	public:
		virtual ~EventBase() = default;

		const bool inline IsHandled() const { return m_Handled; }
		void SetIsHandled(bool value) { m_Handled = value; }

		virtual std::string ToString() const = 0;

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