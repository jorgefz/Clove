#pragma once
#include "clovepch.h"

/*
	Platform detection.
	Currently, only 64-bit Windows is suported.
*/
#ifdef _WIN32
	#ifdef _WIN64 /* Windows x64 */
		#include "Windows.h"
	#else /* Windows x86 */
		#error "x86 builds are not supported"
	#endif
#else
	#error "Clove only supports Windows"
#endif


/*
	Debug information macros, only used in DEBUG configuration.
	Format messages using c-style printf functions.
	E.g. CLOVE_INFO("This resolution might cause issues: %ux%u", width, height);

	CLOVE_ERROR raises a debug break and interrupts your program.
	CLOVE_ASSERT calls CLOVE_ERROR if the condition is met.
	CLOVE_WARN prints out a warning message that starts with [WARNING]
	CLOVE_INFO prints out a message that starts with [INFO]
*/
#ifdef CLOVE_DEBUG
#	ifndef CLOVE_DEBUG_OUT /* file stream to output debug information to */
#		define CLOVE_DEBUG_OUT stderr
#	endif
#	define CLOVE_ERROR(msg, ...) do { fprintf(CLOVE_DEBUG_OUT, " [ERROR] " msg "\n", ##__VA_ARGS__); __debugbreak(); } while(0);
#	define CLOVE_ASSERT(condition, msg, ...) if (!(condition)) { fprintf(CLOVE_DEBUG_OUT, " [ERROR] " msg " \n", ##__VA_ARGS__); __debugbreak(); }
#	define CLOVE_WARN(msg, ...) fprintf(CLOVE_DEBUG_OUT, " [WARNING] " msg " \n", ##__VA_ARGS__)
#	define CLOVE_INFO(msg, ...) fprintf(CLOVE_DEBUG_OUT, " [INFO] " msg "\n", ##__VA_ARGS__)
#else
#	define CLOVE_ERROR(msg, ...)
#	define CLOVE_ASSERT(condition, msg, ...) condition
#	define CLOVE_WARN(msg, ...)
#	define CLOVE_INFO(msg, ...)
#endif


// Mostly used to use class methods as event callbacks, e.g. this->EventCallback(Event& e) 
#define CLOVE_BIND_METHOD_1(fn) std::bind(&fn, this, std::placeholders::_1)


/*
	Reference Counting System.
	Currently, a Scope behaves like an unique_ptr,
	and a Ref as a share_ptr.
*/
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

