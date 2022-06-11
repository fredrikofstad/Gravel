#pragma once

#ifdef GR_PLATFORM_WINDOWS
#if GR_DLL
	#ifdef GR_BUILD_DLL
		#define GRAVEL_API __declspec(dllexport)
	#else
		#define GRAVEL_API __declspec(dllimport)
	#endif
#else
	#define GRAVEL_API
#endif
#else
	#error platform not supported
#endif

#ifdef GR_DEBUG
	#define GR_ENABLE_ASSERTS
#endif

#ifdef GR_ENABLE_ASSERTS
	#define GR_ASSERTS(x, ...) { if(!(x)){ GR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GR_CORE_ASSERT(x, ...) { if(!(x)){ GR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GR_ASSERTS(x, ...)
	#define GR_CORE_ASSERT(x, ...)

#endif

#define BIT(x) (1 << x)

#define GR_BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)
