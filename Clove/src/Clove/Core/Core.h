#pragma once
#include "clovepch.h"

// Platform

#ifdef CLOVE_WINDOWS
#	include "windows.h"
#else
//#	error Clove only supports Windows
#endif


// Macros
#ifdef CLOVE_DEBUG
#	define CLOVE_ASSERT(condition, msg) if(!(condition)) { throw std::runtime_error(msg); }
#	define CLOVE_WARN(condition, msg) if(!(condition)) { std::cerr << msg << std::endl; }
#	define CLOVE_INFO(msg) std::cerr << msg << std::endl;
#else
#	define CLOVE_ASSERT(condition, msg) condition;
#	define CLOVE_WARN(condition, msg) condition;
#	define CLOVE_INFO(msg)
#endif

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

