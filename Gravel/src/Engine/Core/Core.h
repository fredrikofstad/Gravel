#pragma once

#include <memory>

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

#define GR_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace Gravel {


	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Unique<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Shared<T> MakeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	using RendererID = uint32_t;


}

