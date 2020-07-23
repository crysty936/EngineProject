#pragma once


#include "pch.h"
#include "Core.h"
#include "spdlog\spdlog.h"
#include "spdlog\sinks\stdout_color_sinks.h"
#include "spdlog\fmt\ostr.h"

namespace Engine {

	class ENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){ return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientCoreLogger(){ return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;


	};
}

//Core Log Macros
#define CORE_ERROR(...)		::Engine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)		::Engine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)		::Engine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...)		::Engine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_CRITICAL(...)	::Engine::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Core Client Macros
#define ERROR(...)		::Engine::Log::GetClientCoreLogger()->error(__VA_ARGS__)
#define WARN(...)		::Engine::Log::GetClientCoreLogger()->warn(__VA_ARGS__)
#define INFO(...)		::Engine::Log::GetClientCoreLogger()->info(__VA_ARGS__)
#define TRACE(...)		::Engine::Log::GetClientCoreLogger()->trace(__VA_ARGS__)
#define CRITICAL(...)	::Engine::Log::GetClientCoreLogger()->critical(__VA_ARGS__)


