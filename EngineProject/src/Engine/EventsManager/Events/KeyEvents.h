#pragma once

#include "Engine/EventsManager/Events/EventBase.h"
#include "Engine/Core/Input.h"

namespace Engine
{
	class KeyEventBase : public EventBase
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

		virtual int GetCategoryFlags() const { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; }

	protected:
		KeyEventBase(int keycode) :m_KeyCode(keycode) {}
		int m_KeyCode;
	};

	class KeyEvent :public KeyEventBase
	{
	public:
		KeyEvent(int keycode)
			:KeyEventBase(keycode) {}

		static std::string GetStaticName() { return "KeyEvent"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << m_KeyCode;
			return ss.str();
		}

	};

	class KeyPressedEvent : public KeyEventBase
	{
	public:
		KeyPressedEvent(int keycode)
			:KeyEventBase(keycode) {}

		static std::string GetStaticName() { return "KeyPressedEvent"; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << GetStaticName() << m_KeyCode;
			return ss.str();
		}
	};

	class KeyReleasedEvent : public KeyEventBase
	{
	public:
		KeyReleasedEvent(int keycode)
			:KeyEventBase(keycode) {}
		static std::string GetStaticName() { return "KeyReleasedEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << ": " << m_KeyCode;
			return ss.str();
		}
	};
	class KeyRepeatEvent : public KeyEventBase
	{
	public:
		KeyRepeatEvent(int keycode, int repeatCount)
			:KeyEventBase(keycode), RepeatCount(repeatCount) {}

		static std::string GetStaticName() { return "KeyRepeatEvent"; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << GetStaticName() << m_KeyCode;
			return ss.str();
		}
		int RepeatCount;
	};

	class KeyTypedEvent : public KeyEventBase
	{
	public:
		KeyTypedEvent(int keycode)
			:KeyEventBase(keycode) {}
		static std::string GetStaticName() { return "KeyTypedEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << ": " << m_KeyCode;
			return ss.str();
		}
	};
}



