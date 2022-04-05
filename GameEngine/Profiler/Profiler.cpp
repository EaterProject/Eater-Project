#pragma warning(disable: 4996)

#include "Profiler.h"
#include "Debugger.h"

Debugger g_Debugger;

PROFILER_DLL bool Create()
{
	static bool isCreate = false;

	if (!isCreate)
	{
		isCreate = true;
		g_Debugger.Create();
	}

	return true;
}

PROFILER_DLL void Log(PROFILE_OUTPUT outputType, long result, const char* file, const char* func, int&& line, const char* message, ...)
{
	std::string fileinfo = g_Debugger.GetFileInfo(file, func, line);

	int length;
	va_list args;

	va_start(args, message);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(message, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, message, args);

	va_end(args);

	g_Debugger.Log(outputType, result, fileinfo.c_str(), cBuf, length);

	delete[] cBuf;
}

PROFILER_DLL void TimerStart(PROFILE_OUTPUT outputType, const char* file, const char* func, int&& line, int&& totalFrame, const char* timerKey, ...)
{
	int length;
	va_list args;

	va_start(args, timerKey);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(timerKey, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, timerKey, args);

	va_end(args);

	g_Debugger.TimerStart(outputType, file, func, line, totalFrame, cBuf);

	delete[] cBuf;
}

PROFILER_DLL void TimerEnd(const char* timerKey, ...)
{
	int length;
	va_list args;

	va_start(args, timerKey);

	// 가변인자 문자열 길이 측정..
	length = _vscprintf(timerKey, args) + 1;

	// 가변인자 문자열 변환..
	char* cBuf = new char[length];
	memset(cBuf, NULL, length);

	vsprintf(cBuf, timerKey, args);

	va_end(args);

	g_Debugger.TimerEnd(cBuf);

	delete[] cBuf;
}
