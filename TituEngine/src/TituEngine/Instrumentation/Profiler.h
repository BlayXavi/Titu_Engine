#pragma once

#include <string>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std::chrono;

namespace TituEngine
{

#if TE_ENABLE_PROFILER
#define PROFILE(name) TituEngine::Profiler onUpdatePerformance(##name);
#else
#define PROFILE(name)
#endif

	struct ProfileResult
	{
		ProfileResult() = default;

		ProfileResult(const char* name, float time)
			:Name(name), Time(time)
		{
		}

		const char* Name;
		float Time;
	};

	class Profiler
	{
	public:

		Profiler(const char* name);
		~Profiler();

		void Stop();
		static void EndFrame();

		static inline std::vector<ProfileResult>& GetProfilerResults() { return m_ProfilerResults; }
		static inline int GetProfilerCount() { return m_ProfilerCount; }
	private:
		const char* m_Name;
		bool m_Stopped;

		time_point<steady_clock> m_StartTimepoint;

		static std::vector<ProfileResult> m_ProfilerResults;
		static int m_ProfilerCount;
	};
}
