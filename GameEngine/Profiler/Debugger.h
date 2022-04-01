#pragma once
#include <Windows.h>
#include <chrono>
#include <string>
#include <map>

typedef size_t Hash;
typedef std::chrono::system_clock::time_point Clock;

template<typename T>
using Time = std::chrono::duration<T>;

struct TIME_DESC
{
	DEBUG_OUTPUT m_OutputType;

	std::string m_Location;

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
	void TimerStart(DEBUG_OUTPUT outputType, const char* func, int& line, const char* timeKey, int& totalFrame);
	void TimerEnd(const char* key);

	void Log(DEBUG_OUTPUT& outputType, const char* message, ...);
	void Log(DEBUG_OUTPUT& outputType, char* message, int length);

private:
	std::map<Hash, TIME_DESC>::iterator m_NowKey;
	std::map<Hash, TIME_DESC> m_Timer;

	HANDLE m_Console;

	UINT m_LogCount;
};