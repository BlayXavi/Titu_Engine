#include "tepch.h"
#include "Profiler.h"

#include "imgui.h"

namespace TituEngine
{
	Profiler::Profiler(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = high_resolution_clock::now();
	}

	Profiler::~Profiler()
	{
		if (!m_Stopped)
			Stop();
	}

	void Profiler::Stop()
	{
		m_Stopped = true;

		auto endTimepoint = high_resolution_clock::now();

		long long start = time_point_cast<microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();

		uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
	}
}
