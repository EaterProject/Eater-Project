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
	PROFILE_OUTPUT OutputType;

	std::string Location;

	UINT NowFrame;
	UINT TotalFrame;

	Time<double> TotalTime;

	Clock Start;
};

struct LOG_DESC
{
	std::string LogName;
	FILE* LogFile;
};

class Debugger
{
public:
	Debugger();

public:
	void TimerStart(PROFILE_OUTPUT outputType, const char* func, int& line, int& totalFrame, std::string&& timerKey);
	void TimerEnd(std::string&& timerKey);

	void Log(PROFILE_OUTPUT& outputType, const char* message, ...);
	void Log(PROFILE_OUTPUT& outputType, const char* fileInfo, char* message, int length);

public:
	std::string GetFileInfo(const char* file, const char* func, int& line);

private:
	std::string GetTime();

private:
	std::map<Hash, TIME_DESC>::iterator m_NowKey;
	std::map<Hash, TIME_DESC> m_Timer;

	LOG_DESC m_Log;

	HANDLE m_Console;

	UINT m_LogCount;
};