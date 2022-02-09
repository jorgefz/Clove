#pragma once
#include "clovepch.h"

// Platform

#ifdef CLOVE_WINDOWS
#	include "windows.h"
#else
//#	error Clove only supports Windows
#endif


// Debug message macros: MESSAGE(condition, c-style string format, varargs)
// e.g. CLOVE_ASSERT(false, "The value is too large (%f)", 10.0f); 
#ifdef CLOVE_DEBUG
#	define CLOVE_ASSERT(condition, msg, ...) if (!(condition)) { printf(" [ERROR] " msg " \n", ##__VA_ARGS__); __debugbreak(); }
#	define CLOVE_WARN(condition, msg, ...) if(!(condition)) { printf(" [WARNING] " msg " \n", ##__VA_ARGS__); }
#	define CLOVE_INFO(msg, ...) printf(" [INFO] " msg "\n", ##__VA_ARGS__);
#else
#	define CLOVE_ASSERT(condition, msg, ...) condition;
#	define CLOVE_WARN(condition, msg, ...) condition;
#	define CLOVE_INFO(msg, ...)
#endif


// Mostly used to use class methods as event callbacks, e.g. this->EventCallback(Event& e) 
#define CLOVE_BIND_METHOD_1(fn) std::bind(&fn, this, std::placeholders::_1)


// Reference Counting System
namespace Clove {
	
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

