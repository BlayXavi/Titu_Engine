#pragma once

namespace TituEngine
{
	struct Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return GetDeltaTime(); }

		inline float GetDeltaTime() const { return m_Time; }
		inline float GetDeltaTimeMilliseconds() const { return m_Time * 1000.0f; }

		static float GetTime();

	private:
		float m_Time;
	};
}