#pragma once

#include "pch.h"
#include "Events/EventBase.h"

namespace Engine
{
	class EventDispatcher
	{
	private:
		EventBase& m_Event;
	public:
		EventDispatcher(EventBase& event) :m_Event(event)
		{
		}
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.SetIsHandled(func(static_cast<T&>(m_Event)));
				return true;
			}
			return false;
		}


	};

}