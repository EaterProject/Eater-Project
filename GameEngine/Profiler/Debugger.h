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

	std::string KeyName;
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
	void Create();

public:
	void TimerStart(PROFILE_OUTPUT outputType, const char* file, const char* func, int& line, int& totalFrame, std::string&& timerKey);
	void TimerEnd(std::string&& timerKey);

	void Log(PROFILE_OUTPUT& outputType, HRESULT result, const char* fileInfo, char* message, int length);

public:
	std::string GetFileInfo(const char* file, const char* func, int& line);

private:
	void Log(PROFILE_OUTPUT& outputType, const char* message, ...);

private:
	std::string GetTime();
	std::string GetFileName(const char* file);

private:
	struct TOKEN
	{
		static const std::string Error;
		static const std::string File;
		static const std::string Func;
		static const std::string Line;
		static const std::string Enter;
		static const std::string Space;
		static const std::string Tab;
	};

private:
	std::map<Hash, TIME_DESC>::iterator m_NowKey;
	std::map<Hash, TIME_DESC> m_Timer;

	static LOG_DESC g_Log;
	static HANDLE g_Console;
};