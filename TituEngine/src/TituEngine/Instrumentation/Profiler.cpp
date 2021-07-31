#include "tepch.h"
#include "Profiler.h"

#include "imgui.h"

namespace TituEngine
{

	std::vector<ProfileResult> Profiler::m_ProfilerResults = std::vector<ProfileResult>(5);
	int Profiler::m_ProfilerCount = 0;

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
		auto endTimepoint = high_resolution_clock::now();
		long long start = time_point_cast<microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = time_point_cast<microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		m_ProfilerResults[m_ProfilerCount] = (ProfileResult(m_Name, (end - start) * 0.001f));
		m_ProfilerCount++;
	}

	void Profiler::EndFrame()
	{
		for (int i = 0; i < m_ProfilerCount; i++)
		{
			ProfileResult profilingResult = m_ProfilerResults[i];
			char label[50];
			strcpy(label, profilingResult.Name);
			strcat(label, " %.3fms");
			ImGui::Text(label, profilingResult.Time);
		}
		m_ProfilerCount = 0;
	}
}
