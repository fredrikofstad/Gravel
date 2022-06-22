#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Gravel {

	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_currentSession;
		std::ofstream m_outputStream;
		int m_profileCount;
	public:
		Instrumentor()
			: m_currentSession(nullptr), m_profileCount(0)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_outputStream.open(filepath);
			WriteHeader();
			m_currentSession = new InstrumentationSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_outputStream.close();
			delete m_currentSession;
			m_currentSession = nullptr;
			m_profileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_profileCount++ > 0)
				m_outputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_outputStream << "{";
			m_outputStream << "\"cat\":\"function\",";
			m_outputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_outputStream << "\"name\":\"" << name << "\",";
			m_outputStream << "\"ph\":\"X\",";
			m_outputStream << "\"pid\":0,";
			m_outputStream << "\"tid\":" << result.ThreadID << ",";
			m_outputStream << "\"ts\":" << result.Start;
			m_outputStream << "}";

			m_outputStream.flush();
		}

		void WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_outputStream.flush();
		}

		void WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_startTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_name, start, end, threadID });

			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
		bool m_stopped;
	};
}

#define GR_PROFILE 1
#if GR_PROFILE
#define GR_PROFILE_BEGIN_SESSION(name, filepath) ::Gravel::Instrumentor::Get().BeginSession(name, filepath)
#define GR_PROFILE_END_SESSION() ::Gravel::Instrumentor::Get().EndSession()
#define GR_PROFILE_SCOPE(name) ::Gravel::InstrumentationTimer timer##__LINE__(name);
#define GR_PROFILE_FUNCTION() GR_PROFILE_SCOPE(__FUNCSIG__)
#else
#define GR_PROFILE_BEGIN_SESSION(name, filepath)
#define GR_PROFILE_END_SESSION()
#define GR_PROFILE_SCOPE(name)
#define GR_PROFILE_FUNCTION()
#endif 