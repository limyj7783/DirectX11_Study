#include "../pch.h"
#include "CQTimer.h"

CQTimer::CQTimer()
{
	m_SecondPerCount = 0.0;
	m_DeltaTime = -1.0;

	m_BaseTime = 0;
	m_PausedTime = 0;
	m_PrevTime = 0;
	m_CurrTime = 0;

	m_Stopped = false;

	__int64 countsPerSec;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
	m_SecondPerCount = 1.0f / countsPerSec;
}

float CQTimer::GetTotalTime() const
{
	if (m_Stopped)
		return static_cast<float>(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondPerCount);

	return static_cast<float>(((m_CurrTime - m_PausedTime) - m_BaseTime)*m_SecondPerCount);
}

float CQTimer::GetDeltaTime() const
{
	return static_cast<float>(m_DeltaTime);
}

void CQTimer::Reset()
{
	__int64 _CurrentTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_CurrentTime));

	m_BaseTime = _CurrentTime;
	m_PrevTime = _CurrentTime;
	m_StopTime = 0;

	m_Stopped = false;
}

void CQTimer::Start()
{
	__int64 _StartTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_StartTime));

	if (m_Stopped)
	{
		m_PausedTime += (_StartTime - m_StopTime);

		m_PrevTime = _StartTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
}

void CQTimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 _CurrentTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_CurrentTime));

		m_StopTime = _CurrentTime;
		m_Stopped = true;
	}
}

void CQTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	__int64 _CurrentTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_CurrentTime));
	m_CurrTime = _CurrentTime;

	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondPerCount;
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}
}
