#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Gravel {

	class GRAVEL_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
		{
			return s_coreLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_clientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}
//core log macros
#define GR_CORE_TRACE(...) ::Gravel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GR_CORE_INFO(...)  ::Gravel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GR_CORE_WARN(...)  ::Gravel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GR_CORE_ERROR(...) ::Gravel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GR_CORE_CRITICAL(...) ::Gravel::Log::GetCoreLogger()->critical(__VA_ARGS__)

//client log macros
#define GR_TRACE(...)      ::Gravel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GR_INFO(...)       ::Gravel::Log::GetClientLogger()->info(__VA_ARGS__)
#define GR_WARN(...)       ::Gravel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GR_ERROR(...)      ::Gravel::Log::GetClientLogger()->error(__VA_ARGS__)
#define GR_CRITICAL(...)      ::Gravel::Log::GetClientLogger()->critical(__VA_ARGS__)