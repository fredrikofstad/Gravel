#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>

#include "Engine/Core/Log.h"

namespace Gravel {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_currentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					GR_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_currentSession->Name);
				}
				InternalEndSession();
			}
			m_outputStream.open(filepath);

			if (m_outputStream.is_open())
			{
				m_currentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					GR_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_currentSession)
			{
				m_outputStream << json.str();
				m_outputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		Instrumentor()
			: m_currentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_outputStream.flush();
		}

		void WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_currentSession)
			{
				WriteFooter();
				m_outputStream.close();
				delete m_currentSession;
				m_currentSession = nullptr;
			}
		}
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_currentSession;
		std::ofstream m_outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_stopped = true;
		}
	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define GR_PROFILE 0
#if GR_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define GR_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define GR_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define GR_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define GR_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define GR_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define GR_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define GR_FUNC_SIG __func__
#else
#define GR_FUNC_SIG "GR_FUNC_SIG unknown!"
#endif

#define GR_PROFILE_BEGIN_SESSION(name, filepath) ::Gravel::Instrumentor::Get().BeginSession(name, filepath)
#define GR_PROFILE_END_SESSION() ::Gravel::Instrumentor::Get().EndSession()
#define GR_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Gravel::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::Gravel::InstrumentationTimer timer##line(fixedName##line.Data)
#define GR_PROFILE_SCOPE_LINE(name, line) GR_PROFILE_SCOPE_LINE2(name, line)
#define GR_PROFILE_SCOPE(name) GR_PROFILE_SCOPE_LINE(name, __LINE__)
#define GR_PROFILE_FUNCTION() GR_PROFILE_SCOPE(GR_FUNC_SIG)
#else
#define GR_PROFILE_BEGIN_SESSION(name, filepath)
#define GR_PROFILE_END_SESSION()
#define GR_PROFILE_SCOPE(name)
#define GR_PROFILE_FUNCTION()
#endif