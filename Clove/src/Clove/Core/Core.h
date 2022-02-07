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
#else
#	define CLOVE_ASSERT(condition, msg) condition; 
#endif

#define CLOVE_BIND_METHOD_1(fn) std::bind(&fn, this, std::placeholders::_1)


// Reference Counting System
namespace Clove {
	
	template<typename T>
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
}

