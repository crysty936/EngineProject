#pragma once


#ifdef PLATFORM_WINDOWS
#if DYNAMIC_LINK
#ifdef BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#endif
#else
#error Enhine only supports Windows
#endif


#ifdef ENABLE_ASSERTS
#define ENGINE_ASSERT(x, ...){if(!(x)){LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define ENGINE_CORE_ASSERT(x, ...){if(!(x)){CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ENGINE_ASSERT(x, ...)
#define ENGINE_CORE_ASSERT(x, ...)

#endif // ENABLE_ASSERTS



