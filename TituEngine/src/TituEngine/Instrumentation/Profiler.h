#pragma once

#include <string>
#include <chrono>
#include <iostream>
#include <vector>

#include <algorithm>
#include <fstream>

#include <thread>

using namespace std::chrono;

//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

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
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
public:
	Instrumentor()
		: m_CurrentSession(nullptr), m_ProfileCount(0)
	{
	}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json")
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void WriteProfile(const ProfileResult& result)
	{
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadID << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void WriteHeader()
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	static Instrumentor& Get()
	{
		static Instrumentor instance;
		return instance;
	}

};

namespace TituEngine
{

#if TE_ENABLE_PROFILER
#define TE_PROFILE_BEGIN_SESSION(name, filepath) Instrumentor::Get().BeginSession(name, filepath);
#define TE_PROFILE_END_SESSION() Instrumentor::Get().EndSession();
#define TE_PROFILE_PROFILE_SCOPE(name)	TituEngine::Profiler Profile(##name);
#define TE_PROFILE_PROFILE_FUNC() TE_PROFILE_PROFILE_SCOPE(__FUNCSIG__);

#else
#define TE_PROFILE_BEGIN_SESSION(name, filepath)
#define TE_PROFILE_END_SESSION()
#define TE_PROFILE_PROFILE_SCOPE(name) 
#define TE_PROFILE_PROFILE_FUNC()

#endif

	class Profiler
	{
	public:

		Profiler(const char* name);
		~Profiler();

		void Stop();

	private:

		const char* m_Name;
		bool m_Stopped;
		time_point<high_resolution_clock> m_StartTimepoint;

	};
}
