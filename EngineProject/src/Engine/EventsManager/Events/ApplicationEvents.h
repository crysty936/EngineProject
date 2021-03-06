#pragma once

#include "pch.h"
#include "Engine/EventsManager/Events/EventBase.h"
#include "../../../../vendor/GLFW/include/GLFW/glfw3.h"
//#include "GLFW/glfw3.h"

namespace Engine
{
	class WindowResizeEvent : public EventBase
	{
	private:
		unsigned int m_Width, m_Height;

	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height)
		{

		}
		unsigned int GetWidth() { return m_Width; }
		unsigned int GetHeight() { return m_Height; }

		static std::string GetStaticName() { return "WindowResizeEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " " << m_Width << " " << m_Height;

			return ss.str();
		}
		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}

		//static EventType GetStaticType() { return EventType::WindowResize; }
		//virtual const EventType GetEventType() const override { return GetStaticType(); }
		//virtual const char* GetName() const override
// 		{
// 			return STRINGIFY(GetStaticType());
// 		}
	};

	class WindowBufferResizeEvent : public EventBase
	{
	private:
		unsigned int m_Width, m_Height;

	public:
		WindowBufferResizeEvent(unsigned int width, unsigned int height) :
			m_Width(width), m_Height(height) {}
		unsigned int GetWidth() { return m_Width; }
		unsigned int GetHeight() { return m_Height; }
		static std::string GetStaticName() { return "WindowBufferResizeEvent"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetStaticName() << " " << m_Width << " " << m_Height;

			return ss.str();
		}

		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}
	};


	class WindowCloseEvent : public EventBase
	{
	public:
		WindowCloseEvent(GLFWwindow* _window)
		{
			m_Window = _window;
		}
		GLFWwindow* m_Window = 0;

		std::string ToString() const override
		{
			return GetStaticName();
		}
		static std::string GetStaticName()
		{
			return "WindowCloseEvent";
		}
		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}
		GLFWwindow* GetWindow() { return m_Window; }
	};
	class AppTickEvent : public EventBase
	{
	public:
		AppTickEvent() = default;

		static std::string GetStaticName()
		{
			return "AppTickEvent";
		}

		std::string ToString() const override
		{
			return GetStaticName();
		}
		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}
	};
	class AppUpdateEvent :public EventBase
	{
	public:
		AppUpdateEvent() = default;
		static std::string GetStaticName()
		{
			return "AppUpdateEvent";
		}
		std::string ToString() const override
		{
			return GetStaticName();
		}
		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}
	};
	class AppRenderEvent : public EventBase
	{
		AppRenderEvent() = default;

		static std::string GetStaticName()
		{
			return "AppRenderEvent";
		}
		std::string ToString() const override
		{
			return GetStaticName();
		}
		virtual int GetCategoryFlags() const override
		{
			return static_cast<int>(EventCategory::EventCategoryApplication);
		}
	};
}


