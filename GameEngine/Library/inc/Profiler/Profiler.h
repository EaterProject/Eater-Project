#pragma once
#include "ProfilerDLL.h"

#if defined(DEBUG) || defined(_DEBUG)
	#define PROFILE_LOG(output, message, ...)		Log(output, message, ##__VA_ARGS__)
	#define PROFILE_TIMER_START(output, key, frame)	TimerStart(output, __FUNCTION__, __LINE__, key, frame);
	#define PROFILE_TIMER_END(key)					TimerEnd(key);
#else
	#define PROFILE_LOG(output, message, ...)
	#define PROFILE_TIMER_START(output, key, frame)
	#define PROFILE_TIMER_END(key)					
#endif

PROFILER_DLL void Log(DEBUG_OUTPUT outputType, const char* message, ...);
PROFILER_DLL void TimerStart(DEBUG_OUTPUT outputType, const char* func, int&& line, const char* timeKey, int&& totalFrame);
PROFILER_DLL void TimerEnd(const char* key);