#include "Profiler.h"
#include "Debugger.h"

Debugger g_Debugger = Debugger();

PROFILER_DLL void TimerStart(const char* timeKey, int&& totalFrame, DEBUG_OUTPUT outputType)
{

}

PROFILER_DLL void TimerStart(const char* func, int&& line, const char* timeKey, int&& totalFrame, DEBUG_OUTPUT outputType)
{
#if defined(DEBUG) || defined(_DEBUG)
	g_Debugger.TimerStart(func, line, timeKey, totalFrame, outputType);
#else
	return;
#endif
}

PROFILER_DLL void TimerEnd()
{
#if defined(DEBUG) || defined(_DEBUG)
	g_Debugger.TimerEnd();
#else
	return;
#endif
}
