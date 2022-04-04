#include "grpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Gravel {

	std::shared_ptr<spdlog::logger> Log::s_coreLogger;

	std::shared_ptr<spdlog::logger> Log:: s_clientLogger;

	void Log::Init()
	{
		// from spdlog documentation
		// color-timestamp-name of logger- log message
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("GRAVEL");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("CLIENT");
		s_clientLogger->set_level(spdlog::level::trace);
	}

}
