#pragma once
#include "ProfilerDLL.h"

#define TIMER_START(message, frame, output) TimerStart(__FUNCTION__, __LINE__, message, frame, output);
#define TIMER_END() TimerEnd();

PROFILER_DLL void TimerStart(const char* func, int&& line, const char* timeKey, int&& totalFrame, DEBUG_OUTPUT outputType);
PROFILER_DLL void TimerEnd();

