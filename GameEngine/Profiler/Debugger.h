#pragma once
#include <Windows.h>
#include <chrono>
#include <string>
#include <map>

typedef std::chrono::system_clock::time_point Clock;

template<typename T>
using Time = std::chrono::duration<T>;

struct TIME_DESC
{
	DEBUG_OUTPUT m_OutputType;

	const char* m_Location;

	UINT m_NowFrame;
	UINT m_TotalFrame;

	Time<double> m_TotalTime;

	Clock m_Start;
};

class Debugger
{
public:
	Debugger();

public:
	void TimerStart(const char* func, int& line, const char* timeKey, int& totalFrame, DEBUG_OUTPUT outputType);
	void TimerEnd();

	void Log(DEBUG_OUTPUT& outputType, const char* message, ...);

private:
	std::map<const char*, TIME_DESC>::iterator m_NowKey;
	std::map<const char*, TIME_DESC> m_Timer;

	HANDLE m_Console;

	UINT m_LogCount;
};