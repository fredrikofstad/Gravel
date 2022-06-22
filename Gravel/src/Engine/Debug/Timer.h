#pragma once

#include <chrono>

namespace Gravel {

	template<typename Fn>
	class Timer
	{
	public:
		Timer(const char* name, Fn&& function)
			: m_name(name), m_function(function), m_stopped(false)
		{
			m_startTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_stopped = true;

			float duration = (end - start) * 0.001f;
			m_function({ m_name, duration });
		}
	private:
		const char* m_name;
		Fn m_function;
		std::chrono::time_point<std::chrono::steady_clock> m_startTimepoint;
		bool m_stopped;
	};

}

#if 0
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); })
#endif
