#pragma once

#include "pch.h"
#include "Engine/EventsManager/Events/EventBase.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/Events/MouseEvents.h"
#include "Engine/EventsManager/Events/KeyEvents.h"
#include "Engine/EventsManager/EventDispatcher.h"

#define BIND_FUNC_EVT(fn) [this](auto&&... args) -> decltype(auto){ return this->fn(std::forward<decltype(args)>(args)...); }, this
#define BIND_FUNC_SIMPLE(fn) std::bind(&fn,this,std::placeholders::_1)


namespace Engine
{
	using callbackContainer = std::pair<void*, std::any>;

	class EventManager
	{
	private:
		static EventManager* s_Instance;
		std::unordered_map<std::string, std::list<callbackContainer>*>* m_Map;
		std::unordered_map<std::string, std::unordered_map<void*, std::list<callbackContainer>*>*>* m_TargetedMap;
		bool m_TraceEnabled = true;

		EventManager()
		{
			m_Map = new std::unordered_map<std::string, std::list<callbackContainer>*>();
			m_TargetedMap = new std::unordered_map<std::string, std::unordered_map<void*, std::list<callbackContainer>*>*>();
		}
		~EventManager()
		{
			delete m_TargetedMap;
			delete m_Map;
		}

		template<typename T>
		void RemoveListenerInList(std::list<callbackContainer>* list, std::function<void(T&)> function, void* owner)
		{
			std::list<callbackContainer>::iterator it = list->begin();

			for (auto const& func : *list)
			{
				auto casted = std::any_cast<std::function<void(T&)>>(func.second);

				auto firstPtr = *(long*)(char*)&casted;
				auto secondPtr = *(long*)(char*)&function;

				if (firstPtr == secondPtr && func.first == owner)
				{
					std::list<callbackContainer>::iterator tempit = it;
					list->erase(tempit);
					break;
				}
				it++;
			}
		}
	public:
		static EventManager& GetInstance()
		{
			//  			static EventManager s_Instance;
			//  			return s_Instance;
			return *s_Instance;
		}
		EventManager(EventManager const& var) = delete;
		void operator=(EventManager const& var) = delete;
		static void Init()
		{
			s_Instance = new EventManager();
		}


		template<typename T>
		void AddListener(std::function<void(T&)> function, void* owner)
		{
			std::string evtType = T::GetStaticName();
			auto outValue = m_Map->find(evtType);

			if (outValue == m_Map->end())
			{
				std::list<std::pair<void*, std::any>>* list = new std::list<std::pair<void*, std::any>>();
				list->push_front(make_pair(owner, function));
				std::pair<std::string, std::list<std::pair<void*, std::any>>*> kvp = make_pair(evtType, list);
				m_Map->insert(kvp);
			}
			else
			{
				outValue->second->push_front(make_pair(owner, function));
			}
		}

		template<typename T>
		void RemoveListener(std::function<void(T&)> function, void* owner)
		{
			std::string evtType = T::GetStaticName();
			auto outValue = m_Map->find(evtType);

			if (outValue != m_Map->end())
			{
				auto outList = outValue->second;
				RemoveListenerInList<T>(outList, function, owner);
			}
			else
			{
				Trace("Could not find listener for {0} to remove", evtType);
			}
		}


		template<typename T>
		void AddListener(std::function<void(T&)> function, void* owner, void* target)
		{
			std::string evtType = T::GetStaticName();
			auto outValue = m_TargetedMap->find(evtType);
			if (outValue == m_TargetedMap->end())
			{
				std::unordered_map<void*, std::list<callbackContainer>*>* insideMap = new std::unordered_map<void*, std::list<callbackContainer>*>();
				std::list<callbackContainer>* list = new std::list<callbackContainer>();
				list->push_front(make_pair(owner, function));

				auto kvp = make_pair(target, list);
				insideMap->insert(kvp);

				auto mainKVP = make_pair(evtType, insideMap);
				m_TargetedMap->insert(mainKVP);
			}
			else
			{
				auto insideMap = outValue->second;
				auto outList = insideMap->find(target);
				if (outList == insideMap->end())
				{
					std::list<callbackContainer>* list = new std::list<callbackContainer>();
					list->push_front(make_pair(owner, function));
					auto kvp = make_pair(target, list);
					insideMap->insert(kvp);
				}
				else
				{
					outList->second->push_front(make_pair(owner, function));
				}
			}
		}

		template<typename T>
		void RemoveListener(std::function<void(T&)> function, void* owner, void* target)
		{
			std::string evtType = T::GetStaticName();
			auto outMapValue = m_TargetedMap->find(evtType);
			if (outMapValue != m_TargetedMap->end())
			{
				auto insideMap = outMapValue->second;
				auto outListValue = insideMap->find(target);
				if (outListValue != insideMap->end())
				{
					auto outList = outListValue->second;
					RemoveListenerInList<T>(outList, function, owner);
				}
				else
				{
					Trace("Could not find inside list for {0} event category to remove targeted event listener", evtType);
				}

			}
			else
			{
				Trace("Could not find inside map for {0} event category to remove targeted event listener", evtType);
			}
		}

		template<typename T>
		void SendEvent(T& e, void* target)
		{
			std::string evtType = T::GetStaticName();
			auto outValue = m_TargetedMap->find(evtType);
			if (outValue == m_TargetedMap->end())
			{
				Trace("No listener for targeted event {0} found", evtType);
			}
			else
			{
				auto insideMap = outValue->second;
				auto outList = insideMap->find(target);
				if (outList != insideMap->end())
				{
					auto list = outList->second;

					for (auto const& function : *list)
					{
						std::any_cast<std::function<void(T&)>>(function.second)(e);
					}
				}
				else
				{
					Trace("No listener for targeted event {0} found", evtType);
				}
			}
		}
		template<typename T>
		void SendEvent(T& e)
		{
			std::string evtType(T::GetStaticName());
			auto mapIter = m_Map->find(evtType);

			if (mapIter != m_Map->end())
			{
				std::list<std::pair<void*, std::any>>* listeners = mapIter->second;

				for (auto const& function : *listeners)
				{
					std::any_cast<std::function<void(T&)>>(function.second)(e);
				}
			}
			else
			{
				Trace("Target for {0} not found", evtType);
			}
		}
		template<typename FormatString, typename... Args>
		void Trace(const FormatString& fmt, const Args &... args)
		{
			if (m_TraceEnabled)
				CORE_TRACE(fmt, args...);
		}
		void inline SetTraceEnabled(bool value)
		{
			m_TraceEnabled = value;
		}
	};

}

